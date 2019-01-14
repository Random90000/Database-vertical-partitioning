#include <tbb/tbb.h>
#include <cstdlib>

#include "bab.hpp"

template<class Node> class BABTask : public tbb::task {
    using BoundType = typename Node:: bound_type;
    Node current;
    static Solution* solution;
    static BoundType* best;
    static bool return_first;
    static tbb::spin_mutex solution_mutex;
    static tbb::spin_mutex nodes_mutex;
    bool is_better(BoundType bound){
        tbb::spin_mutex::scoped_lock lock(solution_mutex);
        return Node::is_better(bound, *best);
    }
public:
    template<class N, typename A> friend Solution PBAB(const Matrix&, bool, A);
    BABTask(Node&& node):current(std::move(node)){
    #ifdef DEBUG
        tbb::spin_mutex::scoped_lock lock(solution_mutex);
        std::cout << "task\n";
        for(auto &m : node.submatrices){
            std::cout << m << std::endl;
        }
    #endif
    }
    tbb::task* execute(){
        if(return_first && *best != Node::init_bound())
            return nullptr; // a solution has been found already
        if(!is_better(current.get_bound()))
            return nullptr; // this node is outdated
        BoundType bound;
        std::vector<Matrix>& subs = current.subs();
        #ifdef DEBUG
        std::cerr << "Looking at: " << std::endl;
        for(auto& sub : subs)
            std::cerr << sub << std::endl;
        #endif
        for(int i = 0; !current.is_branching() && i < subs.size();)
            if(Node::valid_submatrix(subs[i]))
                ++i; // go on if valid
            else {
                // if not, apply the CI algorithm
                std::vector<Matrix> clusters = subs[i].clusters();
                if(clusters.size() > 1){
                    // if the submatrix is decomposable, replace it with its clusters
                    // we are supposed to create a new node, but there's no difference
                    subs.erase(subs.begin() + i);
                    subs.insert(subs.begin() + i, make_move_iterator(clusters.begin()), make_move_iterator(clusters.end()));
                    // CI will be called again, but it shouldn't be a problem
                } else {
                    // if the matrix is not decomposable, start branching
                    current.start_branching(i);
                }
            }
        if(current.is_branching()){
            int ref = 0;
            std::vector<Node> children;
            while(current.has_more_nodes()){
                Node child = current.next_node();
                // we do not allow empty submatrices
                if(child.has_empty_rows_cols())
                    continue;
                bound = child.get_bound();
                // skip node if solution will be worse
                if(!is_better(bound))
                    continue;
                children.push_back(std::move(child));
                ++ref;
            }
            if(ref){
                {
                    tbb::spin_mutex::scoped_lock lock(nodes_mutex);
                    nodes += ref;
                }

                set_ref_count(ref+1);
                for(int i = 0; i < children.size(); ++i){
                    BABTask<Node>& child_task = *new(allocate_child()) BABTask(std::move(children[i]));
                    spawn(child_task);
                }
                wait_for_all();
            }
        } else {
            // all good, that's a solution
            bound = current.get_bound();
            {
                tbb::spin_mutex::scoped_lock lock(solution_mutex);
                if(Node::is_better(bound, *best)){
                    *solution = std::move(current);
                    *best = bound;
                }
            }
        }
        return nullptr;
    }
};

template<class Node> Solution* BABTask<Node>::solution;
template<class Node> typename Node::bound_type* BABTask<Node>::best;
template<class Node> bool BABTask<Node>::return_first;
template<class Node> tbb::spin_mutex BABTask<Node>::solution_mutex;
template<class Node> tbb::spin_mutex BABTask<Node>::nodes_mutex;

template<class Node, typename additional> Solution PBAB(const Matrix& m, bool return_first, additional arg = additional()){
    int threads;
    {
        char* var = getenv("TBB_THREADS");
        threads = var ? atoi(var) : 0;
    }
    tbb::task_scheduler_init init(tbb::task_scheduler_init::deferred);
    if(threads) init.initialize(threads);
    else init.initialize();
    Solution solution;
    auto best = Node::init_bound();
    BABTask<Node>::solution = &solution;
    BABTask<Node>::best = &best;
    BABTask<Node>::return_first = return_first;
    ++nodes;
    BABTask<Node>& root = *new(tbb::task::allocate_root()) BABTask<Node>(Node {m, arg});
    tbb::task::spawn_root_and_wait(root);
    return solution;
};

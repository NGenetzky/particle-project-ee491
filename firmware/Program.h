
#pragma once
#include "File.h"
namespace iot {

struct ProgramContext {
    uint8_t state=0;
    uint8_t status=0;
    uint8_t config=0;
    
    std::vector<char> data;
    iot::File *in = nullptr;
    iot::File *out = nullptr;
};

using ProgramTask = std::function<void(iot::ProgramContext&)>;
using ControlFlowFunctor = std::function<std::vector<unsigned>(iot::ProgramContext&)>;

class Program {
    public:
        int run() {
            for( auto i_task : this->active_tasks ){
                if( this->tasks.size() <= i_task ){ break; } // range check.
                this->tasks[i_task]( this->context );
            }
            this->active_tasks = control_flow_f(this->context);
            return this->active_tasks.size();
        }
        
        void set_control_flow( ControlFlowFunctor cf ) {
            this->control_flow_f = cf;
        }
        
        std::vector<unsigned> add_tasks( std::vector<iot::ProgramTask> new_tasks) {
            auto task_indexes = std::vector<unsigned>{};
            for( auto task : new_tasks ){
                task_indexes.push_back(this->tasks.size());
                this->tasks.push_back(task);
            }
            return task_indexes;
        }
    
    private:
        std::vector<iot::ProgramTask> tasks;
        std::vector<unsigned> active_tasks = {};
        iot::ControlFlowFunctor control_flow_f =
            [] ( iot::ProgramContext &m) -> std::vector<unsigned> {
                return {}; // By default don't set any active tasks.
            };
        iot::ProgramContext context;
};

}


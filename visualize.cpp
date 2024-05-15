#include <matplot/matplot.h>
#include "visualize.h"
#define vis_dif 0.00000001 //workaround so matplot displays "one-sliced" pie charts
using namespace matplot;
void visualize_fractions(int sim_steps,int nr_of_resources,vector<vector<vector<function_space>>>& fraction_at_q_at_collection_step_for_type) {
    //tiledlayout(sim_steps, nr_of_resources);
    for (int i = 0; i < nr_of_resources; i++) {
        for (int j = 0; j < fraction_at_q_at_collection_step_for_type[0].size(); j++) {
            for(auto& a: fraction_at_q_at_collection_step_for_type[i][j]) {
                if(a==1.0) a-=vis_dif;
            }
            //auto ax_tmp = nexttile();
            subplot(sim_steps,nr_of_resources,i+j*nr_of_resources);
            pie(fraction_at_q_at_collection_step_for_type[i][j]);
            title("Resource " + to_string(i) + " at step " + to_string(j));
        }
    }
    show();
}
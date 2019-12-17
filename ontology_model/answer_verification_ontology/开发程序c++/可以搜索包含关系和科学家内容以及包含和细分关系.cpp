extern "C" {
#include "sc_memory_headers.h"
#include "sc_helper.h"
#include "utils.h"
}

#include <stdio.h>
#include <iostream>
#include <glib.h>
#include <unistd.h>
#include <assert.h>
#include <vector>
#include <algorithm>

using namespace std;

sc_memory_context *context;

sc_addr concept_scientist, concept_binary_tree,nrel_inclusion,concept_graph,nrel_subdividing;

void print_graph()
{
    int i=0;
 /*   printEl(context, concept_scientist);
    cout << endl << "----------------------" << endl;
        sc_iterator3 *arcs_it = sc_iterator3_f_a_a_new(context,
                                concept_scientist,
                                sc_type_arc_pos_const_perm,
                                0);

        while (SC_TRUE == sc_iterator3_next(arcs_it)) {

            sc_addr t_arc = sc_iterator3_value(arcs_it, 2);
            printEl(context, t_arc);
            cout << endl;
        }
        sc_iterator3_free(arcs_it);
        cout << endl;

        printEl(context, concept_binary_tree);
        cout << endl << "----------------------" << endl;
        sc_iterator5 *its = sc_iterator5_f_a_a_a_f_new(context,
                               concept_binary_tree,
                               sc_type_arc_common,
                               0,
                               sc_type_arc_pos_const_perm,
                               nrel_inclusion);
        while (SC_TRUE == sc_iterator5_next(its))
        {

            sc_addr it_au = sc_iterator5_value(its, 2);
            printEl(context,it_au);
           cout << endl;
        }

         sc_iterator5_free(its);

*/
         printEl(context, concept_graph);
         cout << endl << "inclusion:->->->->->->->->->->->->->->" << endl;
         sc_iterator5 *grp = sc_iterator5_f_a_a_a_f_new(context,
                                concept_graph,
                                sc_type_arc_common,
                                0,
                                sc_type_arc_pos_const_perm,
                                nrel_inclusion);
         while (SC_TRUE == sc_iterator5_next(grp))
         {

             sc_addr grp_cp = sc_iterator5_value(grp, 2);
             printEl(context,grp_cp);
             cout << endl;
             i++;
             sc_iterator5 *sub_grp= sc_iterator5_f_a_a_a_f_new(context,
                                                               grp_cp,
                                                               sc_type_arc_common,
                                                               0,
                                                               sc_type_arc_pos_const_perm,
                                                               nrel_inclusion);
                          while (SC_TRUE== sc_iterator5_next(sub_grp))
                   {
                       sc_addr sub_grp_con = sc_iterator5_value(sub_grp, 2);
                       cout <<"  :**";
                       printEl(context,sub_grp_con);
                       cout << endl;
                   }
             sc_iterator5_free(sub_grp);
                     }
          cout << endl;
          cout <<"number of graphs:"<<i<<endl;
          sc_iterator5_free(grp);



          printEl(context, concept_graph);
          cout << endl << "subdividing:&>&>&>&>&>&>&>&>&>&>&>&>&>&>" << endl;
          sc_iterator5 *grp_subd = sc_iterator5_a_a_f_a_f_new(context,
                                 0,
                                 sc_type_arc_common,
                                 concept_graph,
                                 sc_type_arc_pos_const_perm,
                                 nrel_subdividing);
          while (SC_TRUE == sc_iterator5_next(grp_subd))
          {
             sc_addr grp_subd_con = sc_iterator5_value(grp_subd,0);
            // printEl(context,grp_subd_con);
             cout << "Q";
             cout << endl;
             sc_iterator3 *subd_grp_subd = sc_iterator3_f_a_a_new(context,
                                                                  grp_subd_con,
                                                                  sc_type_arc_pos_const_perm,
                                                                  0);
               while (SC_TRUE == sc_iterator3_next(subd_grp_subd))
                {
                     sc_addr subd_grp_subd_con = sc_iterator3_value(subd_grp_subd,2);
                     cout <<"  :";
                     printEl(context,subd_grp_subd_con);
                     cout << endl;
                }
              sc_iterator3_free(subd_grp_subd);
          }

          cout << endl;
          sc_iterator5_free(grp_subd);
    }

void test()
{
   // sc_helper_resolve_system_identifier(context, "concept_scientist", &concept_scientist);
  //  sc_helper_resolve_system_identifier(context, "concept_binary_tree", &concept_binary_tree);
    sc_helper_resolve_system_identifier(context, "concept_graph", &concept_graph);
    sc_helper_resolve_system_identifier(context, "nrel_subdividing", &nrel_subdividing);
    sc_helper_resolve_system_identifier(context, "nrel_inclusion", &nrel_inclusion);
    cout << "Graph: ";
    print_graph();
    cout << endl;
}

int main()
{
    sc_memory_params params;

    sc_memory_params_clear(&params);
    params.repo_path = "/var/gt.ostis/kb.bin";
    params.config_file = "/var/gt.ostis/config/sc-web.ini";
    params.ext_path = "/var/gt.ostis/sc-machine/bin/extensions";
    params.clear = SC_FALSE;
    sc_memory_initialize(&params);
    context = sc_memory_context_new(sc_access_lvl_make_max);


    test();


    cout << "The end" << endl;

    sc_memory_context_free(context);

    sc_memory_shutdown(SC_TRUE);

    return 0;
}

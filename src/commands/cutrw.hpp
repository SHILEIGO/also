/* also: Advanced Logic Synthesis and Optimization tool
 * Copyright (C) 2019- Ningbo University, Ningbo, China */

/**
 * @file cutrw.hpp
 *
 * @brief cut rewriting
 *
 * @author Zhufei Chu
 * @since  0.1
 */

#ifndef CUTRW_HPP
#define CUTRW_HPP

#include <mockturtle/mockturtle.hpp>
#include "../networks/m5ig/m5ig_npn.hpp"

namespace alice
{

  class cutrw_command : public command
  {
    public:
      explicit cutrw_command( const environment::ptr& env ) : command( env, "Performs cut rewriting" )
      {
        add_flag( "--xmg, -x",            "xmg based resynthesize" );
        add_flag( "--m5ig, -r",           "m5ig based resynthesize" );
      }
      
      template<class Ntk>
        void print_stats( Ntk& ntk )
        {
          depth_view depth_ntk( ntk );
          std::cout << fmt::format( "ntk   i/o = {}/{}   gates = {}   level = {}\n", 
                    ntk.num_pis(), ntk.num_pos(), ntk.num_gates(), depth_ntk.depth() );
        }

      void execute()
      {
        /* parameters */
        if( is_set( "m5ig" ) )
        {
          assert( store<m5ig_network>().size() >= 1 );
          m5ig_network m5ig = store<m5ig_network>().current();

          print_stats( m5ig );

          m5ig_npn_resynthesis resyn;
          cut_rewriting_params ps;
          ps.very_verbose = true;
          ps.cut_enumeration_ps.cut_size = 4u;
          cut_rewriting( m5ig, resyn, ps );
          m5ig = cleanup_dangling( m5ig );

          print_stats( m5ig );

          store<m5ig_network>().extend(); 
          store<m5ig_network>().current() = m5ig;
        }
        else
        {
          assert( store<xmg_network>().size() >= 1 );
          xmg_network xmg = store<xmg_network>().current();

          print_stats( xmg );

          xmg_npn_resynthesis resyn;
          cut_rewriting_params ps;
          ps.cut_enumeration_ps.cut_size = 4u;
          cut_rewriting( xmg, resyn, ps );
          xmg = cleanup_dangling( xmg );

          print_stats( xmg );

          store<xmg_network>().extend(); 
          store<xmg_network>().current() = xmg;
        }
      }
    
    private:
      bool verbose = false;
  };

  ALICE_ADD_COMMAND( cutrw, "Rewriting" )

}

#endif

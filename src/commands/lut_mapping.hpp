/* also: Advanced Logic Synthesis and Optimization tool
 * Copyright (C) 2019- Ningbo University, Ningbo, China */

/**
 * @file lut_mapping.hpp
 *
 * @brief lut_mapping 
 *
 * @author Zhufei Chu
 * @since  0.1
 */

#ifndef LUT_MAPPING_HPP
#define LUT_MAPPING_HPP

#include <mockturtle/mockturtle.hpp>
#include <mockturtle/algorithms/satlut_mapping.hpp>

namespace alice
{

  class lut_mapping_command : public command
  {
    public:
      explicit lut_mapping_command( const environment::ptr& env ) : command( env, "LUT mapping" )
      {
        add_option( "cut_size, -k", cut_size, "set the cut size from 2 to 8, default = 4" );
        add_flag( "--verbose, -v", "print the information" );
        add_flag( "--satlut, -s",  "satlut mapping" );
<<<<<<< HEAD
        add_flag( "--James, -j", "This is Leo James,another name--LEI SHI" );     
=======
        add_flag( "--xmg, -x",  "LUT mapping for XMG" );
        add_flag( "--mig, -m",  "LUT mapping for MIG" );
>>>>>>> 98312f42fd3e458d2981245e4405b81109d04bb5
      }

      rules validity_rules() const
      {
        return { has_store_element<aig_network>( env ) };
      }

    protected:
      void execute()
      {
        lut_mapping_params ps;
        
        if( is_set( "xmg" ) )
        {
          /* derive some XMG */
          assert( store<xmg_network>().size() > 0 );
          xmg_network xmg = store<xmg_network>().current();

          mapping_view<xmg_network, true> mapped_xmg{xmg};
          ps.cut_enumeration_ps.cut_size = cut_size;
          lut_mapping<mapping_view<xmg_network, true>, true>( mapped_xmg, ps );
          
          /* collapse into k-LUT network */
          auto klut = *collapse_mapped_network<klut_network>( mapped_xmg );
          klut = cleanup_dangling( klut );
          store<klut_network>().extend(); 
          store<klut_network>().current() = klut;
        }
        else if( is_set( "mig" ) )
        {
          /* derive some MIG */
          assert( store<mig_network>().size() > 0 );
          mig_network mig = store<mig_network>().current();

          mapping_view<mig_network, true> mapped_mig{mig};
          ps.cut_enumeration_ps.cut_size = cut_size;
          lut_mapping<mapping_view<mig_network, true>, true>( mapped_mig, ps );
          
          /* collapse into k-LUT network */
          const auto klut = *collapse_mapped_network<klut_network>( mapped_mig );
          store<klut_network>().extend(); 
          store<klut_network>().current() = klut;
        }
        else
        {
          /* derive some AIG */
          aig_network aig = store<aig_network>().current();

          mapping_view<aig_network, true> mapped_aig{aig};

          /* LUT mapping */
          if( is_set( "satlut" ) )
          {
            satlut_mapping_params ps;
            ps.cut_enumeration_ps.cut_size = 4;
            satlut_mapping<mapping_view<aig_network, true>, true>(mapped_aig, ps );
          }
          else
          {
            ps.cut_enumeration_ps.cut_size = 4;
            lut_mapping<mapping_view<aig_network, true>, true>( mapped_aig, ps );
          }

          /* collapse into k-LUT network */
          const auto klut = *collapse_mapped_network<klut_network>( mapped_aig );
          store<klut_network>().extend(); 
          store<klut_network>().current() = klut;
        }
        
      }
    private:
      unsigned cut_size = 4u;
  };

  ALICE_ADD_COMMAND( lut_mapping, "Mapping commands" )
}

#endif

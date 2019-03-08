/* also: Advanced Logic Synthesis and Optimization tool
 * Copyright (C) 2019- Ningbo University, Ningbo, China 
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * @file exact_imp.hpp
 *
 * @brief exact synthesis to generate an imply logic network
 *
 * @author Zhufei Chu
 * @since  0.1
 */

#ifndef EXACT_IMP_HPP
#define EXACT_IMP_HPP

#include <alice/alice.hpp>
#include <mockturtle/mockturtle.hpp>
#include <percy/percy.hpp>

#include "../store.hpp"
#include "../core/exact_img.hpp"
#include "../encoders/img_encoder.hpp"

using namespace percy;

namespace alice
{
  
  class exact_imply_command: public command
  {
    public:
      explicit exact_imply_command( const environment::ptr& env ) : 
                           command( env, "using exact synthesis to find optimal imgs" )
      {
        add_flag( "--verbose, -v",  "print the information" );
        add_flag( "--img_encoder, -i",  "using the img encoder for synthesize" );
      }

      rules validity_rules() const
      {
        return { has_store_element<optimum_network>( env ) };
      }

    private:
      void nbu_img_encoder_test( const kitty::dynamic_truth_table& tt )
      {
        bsat_wrapper solver;
        spec spec;

        auto copy = tt;
        if( copy.num_vars()  < 2 )
        {
          spec[0] = kitty::extend_to( copy, 2 );
        }
        else
        {
          spec[0] = tt;
        }

        also::img_encoder encoder( solver );

        if ( also::implication_syn_by_img_encoder( spec, solver, encoder ) == success )
        {
          std::cout << " success " << std::endl;
        }
        else
        {
          std::cout << " fail " << std::endl;
        }
      }


    protected:
      void execute()
      {
        bool verb = false;

        if( is_set( "verbose" ) )
        {
          verb = true;
        }

        auto& opt = store<optimum_network>().current();

        if( is_set( "img_encoder" ) )
        {
          nbu_img_encoder_test( opt.function );
        }
        else
        {
          also::implication_syn_by_aig_encoder( opt.function, verb );
        }
      }

  };

  ALICE_ADD_COMMAND( exact_imply, "Exact synthesis" )
}

#endif

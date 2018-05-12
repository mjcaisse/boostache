/**
 *  \file detail/select_context.hpp
 *
 *  Copyright 2015 - 2017 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_VM_DETAIL_SELECT_CONTEXT_HPP
#define BOOST_BOOSTACHE_VM_DETAIL_SELECT_CONTEXT_HPP

#include <boost/boostache/vm/engine_ast.hpp>
#include <boost/boostache/model/select_traits.hpp>
#include <boost/boostache/detail/unwrap_variant_visitor.hpp>

/**
 *  Selecting a context generally moves down in the user's data tree.
 *  A select changes the current data context for subsequent template
 *  commands.
 *
 *  Many of the template languages allow rendering of data up the
 *  data tree. That is a rule for rendering or testing... it doesn't
 *  change the context. Mustache is an example of a template language
 *  that has a select.
 *
 *  Category rules:
 *
 *    - unused      - generate with current context
 *    - plain       - generate with current context
 *    - sequence    - generate with current context (TODO... index?)
 *    - associative - generate with the value returned from the key lookup
 *    - tuple       - generate with current context (TODO... index?)
 *    - variant     - select again after unwrapping
 *    - optional    - generate with current context
 *
 *  TODO : Can mustache move back up the data tree on a select??
 */
namespace boost { namespace boostache { namespace vm { namespace detail
{
   template <typename Stream, typename Context, typename Category>
   void select_context( Stream & stream
                      , ast::select_context const & templ
                      , Context const & ctx
                      , Category)
   {
      generate(stream, templ.body, ctx);
   }


   template <typename Stream, typename Context>
   void select_context( Stream & stream
                      , ast::select_context const & templ
                      , Context const & ctx
                      , extension::variant_attribute)
   {
      boost::apply_visitor(
         [&stream,&templ,&ctx](auto const & unwrapped_ctx)
         {
            select_context( stream, templ, make_context(ctx, unwrapped_ctx)
                          , extension::select_category_t<decltype(unwrapped_ctx)>{});
         }
         , ctx);
   }


   template <typename Stream, typename Context>
   void select_context( Stream & stream
                      , ast::select_context const & templ
                      , Context const & ctx
                      , extension::associative_attribute)
   {
      auto iter = ctx.user_ctx_.find(templ.tag);
      if(iter != ctx.user_ctx_.end())
      {
         generate(stream, templ.body, make_context(ctx, iter->second));
      }
      else
      {
         generate(stream, templ.body, ctx);
      }
   }
   // ------------------------------------------------------------------
   // ------------------------------------------------------------------

}}}}


#endif

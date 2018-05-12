/**
 *  \file detail/engine_visitor.hpp
 *
 *  Copyright 2014, 2015 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_VM_DETAIL_ENGINE_VISITOR_HPP
#define BOOST_BOOSTACHE_VM_DETAIL_ENGINE_VISITOR_HPP

#include <boost/boostache/vm/engine_ast.hpp>
#include <boost/boostache/vm/detail/context_wrapper.hpp>
#include <boost/boostache/vm/detail/foreach.hpp>
#include <boost/boostache/vm/detail/select_context.hpp>
#include <boost/boostache/model/select_traits.hpp>


////////// some test hackery ////////////////////////

namespace boost { namespace boostache { namespace extension
{
   template <typename Stream, typename Object>
   void render(Stream & stream, Object const & v)
   {
      stream << v;
   }

   template <typename T>
   bool test(T const & context, std::string const & name);

   template <typename T>
   bool test(T const & context);

   template< typename Stream, typename T >
   void render(Stream & stream, T const & context, std::string const & name);
}}}

/////////////////////////////////////////////////////


namespace boost { namespace boostache { namespace vm { namespace detail
{
   template <typename Stream, typename Context>
   class engine_visitor_base
   {
   public:
      typedef void result_type;

      engine_visitor_base(Stream & s, Context const & c)
         : stream(s)
         , context(c)
      {}

      void operator()(ast::undefined) const
      {}

      void operator()(ast::nop) const
      {}

      void operator()(ast::literal const & lit) const
      {
         using boost::boostache::extension::render;
         render(stream, lit.value);
      }

      void operator()(ast::variable const &) const
      {}

      void operator()(ast::render const & r) const
      {
         //using boost::boostache::extension::render;
         //render(stream, context, r.name);
         context.render(stream, r.name);
      }

      void operator()(ast::for_each const & v) const
      {
         using boost::boostache::vm::detail::foreach;
         foreach(stream, v, context);
      }

      void operator()(ast::if_then_else const & v) const
      {
         //using boost::boostache::extension::test;
         if(context.test(v.condition_.name))
         {
            boost::apply_visitor(*this, v.then_);
         }
         else
         {
            boost::apply_visitor(*this, v.else_);
         }
      }

      void operator()(ast::select_context const & select_ctx) const
      {
         select_context( stream, select_ctx, context
                       , typename Context::select_category_t{} );
      }

      void operator()(ast::node_list const & nodes) const
      {
         for(auto const & node : nodes.nodes)
         {
            boost::apply_visitor(*this, node);
         }
      }

      void operator()(ast::node const & node) const
      {
         boost::apply_visitor(*this, node);
      }

   private:
      Stream & stream;
      Context const & context;
   };


   template <typename Stream, typename Template, typename P, typename T>
   void generate( Stream & stream
                , Template const & templ
                , vm::detail::context_wrapper<P,T> const & ctx)
   {
      engine_visitor_base<Stream,detail::context_wrapper<P,T>> engine(stream, ctx);
      engine(templ);
   }

}}}}

#endif

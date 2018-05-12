/**
 *  \file context_wrapper.hpp
 *
 *  Copyright 2017 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_VM_DETAIL_CONTEXT_WRAPPER_HPP
#define BOOST_BOOSTACHE_VM_DETAIL_CONTEXT_WRAPPER_HPP
#include <boost/boostache/model/category.hpp>
#include <boost/boostache/model/render_traits.hpp>
#include <boost/boostache/model/test_traits.hpp>
#include <boost/boostache/model/basic_test_extension.hpp>
#include <boost/boostache/model/basic_render_extension.hpp>

namespace boost { namespace boostache { namespace vm { namespace detail
{
   struct no_parent
   {
      bool test(std::string const &) const
      {
         return false;
      }

      bool test() const
      {
         return false;
      }

      template <typename Stream>
      bool render(Stream &, std::string const &) const
      {
         return false;
      }

      template <typename Stream>
      bool select(Stream &, std::string const &) const
      {
         return false;
      }
   };

   template <typename P, typename T>
   struct context_wrapper
   {
      P const & parent_wrapper_;
      T const & user_ctx_;

      //using select_category_t = extension::select_category_t<T>;

      bool test(std::string const & tag_name) const
      {
         using boost::boostache::extension::test;
         auto result = test(user_ctx_, tag_name);
         if(result)
         {
            return *result;
         }
         else
         {
            return parent_wrapper_.test(tag_name);
         }
      }

      bool test() const
      {
         using boost::boostache::extension::test;
         return test(user_ctx_);
      }

      template <typename Stream>
      bool render(Stream & stream, std::string const & tag_name) const
      {
         using boost::boostache::extension::render;
         render( stream, user_ctx_, tag_name
               , extension::render_category_t<T>{});

         return true;
      }

      template <typename Stream>
      bool select(Stream & stream, std::string const & tag_name) const
      {
         // using boost::boostache::extension::select_context;
         // if(!select_context( user_ctx_, tag_name,
         //                   , extension::select_category_t<T>{} ))
         // {
         //    return parent_context_wrapper.select(tag_name);
         // }

         return true;
      }
   };


   template <typename P, typename T>
   auto make_context(P const & parent_, T const & context_)
   {
      return context_wrapper<P,T>{parent_, context_};
   }

   template <typename T>
   auto make_context(T const & context_)
   {
      return context_wrapper<detail::no_parent,T>{detail::no_parent{}, context_};
   }

}}}}

#endif  // BOOST_BOOSTACHE_VM_DETAIL_CONTEXT_WRAPPER_HPP

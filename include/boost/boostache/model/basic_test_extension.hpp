/**
 *  \file basic_test_extension.hpp
 *
 *  Copyright 2014 - 2017 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_MODEL_BASIC_TEST_EXTENSION_HPP
#define BOOST_BOOSTACHE_MODEL_BASIC_TEST_EXTENSION_HPP

#include <boost/optional.hpp>
#include <boost/boostache/vm/traits.hpp>
#include <boost/boostache/model/category.hpp>
#include <boost/boostache/model/test_traits.hpp>
#include <type_traits>


namespace boost { namespace boostache { namespace extension
{
   template <typename T>
   using optional_t = boost::optional<T>;

   namespace detail
   {
      template <typename T>
      bool has_value(boost::optional<T> & v)
      {
         return v != boost::none;
      }

      template <typename T>
      auto get_value(boost::optional<T> & v) -> T&
      {
         return get<T>(v);
      }
   }

   // --------------------------------------------------------------------------
   // Test operations.
   // --------------------------------------------------------------------------
   template <typename T>
   bool test(T const & context);

   template <typename T>
   bool test( T const & context
            , unused_attribute)
   {
      return false;
   }

   template <typename T>
   bool test( T const & context
            , plain_attribute)
   {
      return !!context;
   }

   template <typename T>
   bool test( T const & context
            , sequence_attribute)
   {
      return !context.empty();
   }

   template <typename T>
   bool test( T const & context
            , optional_attribute)
   {
      // if we have an object, test it
      if(context)
      {
         return test(*context);
      }
      else
      {
         return false;
      }
   }

   template <typename T>
   bool test( T const & context
            , associative_attribute)
   {
      return !context.empty();
   }

   template <typename T>
   bool test( T const & context
            , tuple_attribute)
   {
      // TODO : If tuple size is greater than 0, true?
      return true;
   }

   template <typename T>
   bool test( T const & context
            , variant_attribute)
   {
      return true;
      return boost::apply_visitor([](auto const & v) -> bool
                                  {
                                     return test(v);
                                  },
                                  context);
   }

   // --------------------------------------------------------------------------
   // Test with tag
   // Testing with a tag can fail to find or resolve the tag adequetly. As a
   // result, optional<bool> is used as the return type. none response means
   // the tag could not be discovered properly.
   // --------------------------------------------------------------------------

   template <typename T>
   optional_t<bool> test(T const & context, std::string const & tag);


   template <typename T>
   optional_t<bool> test( T const & context, std::string const & tag
                       , unused_attribute)
   {
      return test(context, unused_attribute{});
   }

   template <typename T>
   optional_t<bool> test( T const & context, std::string const & tag
                        , plain_attribute)
   {
      return test(context, plain_attribute{});
   }

   template <typename T>
   optional_t<bool> test( T const & context, std::string const & tag
                        , sequence_attribute)
   {
      return test(context, sequence_attribute{});
   }

   template <typename T>
   optional_t<bool> test( T const & context, std::string const & tag
                        , optional_attribute)
   {
      // if we have an object, test it
      if(context)
      {
         return test(*context, tag);
      }
      else
      {
         return false;
      }
   }

   template <typename T>
   optional_t<bool> test( T const & context, std::string const & tag
                        , variant_attribute)
   {
      return boost::apply_visitor([&tag](auto const & v)
                                  {
                                     return test(v,tag);
                                  },
                                  context);
   }

   template <typename T>
   optional_t<bool> test( T const & context, std::string const & tag
                        , associative_attribute)
   {
      auto iter = context.find(tag);
      if(iter!=context.end())
      {
         return test( iter->second
                    , test_category_t<decltype(iter->second)>{});
      }
      else
      {
         return optional_t<bool>{};
      }
   }


   // --------------------------------------------------------------------------
   // --------------------------------------------------------------------------

   template <typename T>
   bool test(T const & context)
   {
      return test( context
                 , test_category_t<T>{});
   }

   template <typename T>
   optional_t<bool> test(T const & context, std::string const & tag)
   {
      return test( context
                 , tag
                 , test_category_t<T>{});
   }

}}}

#endif

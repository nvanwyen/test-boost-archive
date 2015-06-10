#ifndef __ITEM_HPP
#define __ITEM_HPP

#include <string>
#include <vector>
#include <boost/serialization/access.hpp>

#include "thing.hpp"

//
class item
{
    public:
        //
        struct subitem
        {
            std::string name;
            std::string value;
            things      other;

            //
            subitem() {}    // default construtor is needed for boost::archive::load
            subitem( std::string n, std::string v )
                : name( n ), value( v ) {}

            // only the serialize is needed here, becasue "everything" is public
            template <typename Archive>
            void serialize(Archive &ar, const unsigned int version)
            {
                ar & name;
                ar & value;
                ar & other;
            }
        };

        //
        item( std::string n )
            : name_( n ) {}

        typedef std::vector<subitem> subitems;

        std::string get_name() { return name_; }
        void set_name( std::string n ) { name_ = n; }

        subitems& get_subitems() { return subitems_; }
        void set_subitems( subitems& sub ) { subitems_.insert( subitems_.begin(), sub.begin(), sub.end() ); }

    protected:
    private:
        //
        std::string name_;
        subitems subitems_;

        //
        item() {}

        // need friendship specified, so boost:;archive can access private membership
        friend class boost::serialization::access;

        template <typename Archive>
        void serialize(Archive &ar, const unsigned int version)
        {
            ar & name_;
            ar & subitems_;
        }
};

typedef std::vector<item> items;

#endif // __ITEM_HPP

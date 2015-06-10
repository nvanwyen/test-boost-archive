#ifndef __THING_HPP
#define __THING_HPP

#include <string>
#include <vector>

#include <boost/serialization/access.hpp>

//
class thing
{
    public:
        thing( std::string m )
            : more_( m ) {}

        std::string& get_more() { return more_; }
        void set_more( std::string m ) { more_ = m; }

        std::vector<std::string>& get_stuff() { return stuff_; }
        void set_stuff( std::vector<std::string> s ) { stuff_ = s; }

        void add_stuff( std::string a ) { stuff_.push_back( a ); }

    protected:
    private:
        //
        std::string more_;
        std::vector<std::string> stuff_;

        // defualt constructor (can be public, protected or private
        // ... just needs to exist)
        thing() {}

        // 
        friend class boost::serialization::access;

        template <typename Archive>
        void serialize(Archive &ar, const unsigned int version)
        {
            ar & more_;
            ar & stuff_;
        }
};

typedef std::vector<thing> things;

#endif // __THING_HPP

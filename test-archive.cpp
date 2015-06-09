// test POC for boost serialziation

//
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <vector>

#define TEST_TEMPLATES
//#define USE_TEXT_ARCHIVE      // slower, but smaller output for small classes
#define USE_BINARY_ARCHIVE      // faster, but larger output for small classes
                                // smaller size for large nested classes

//
#ifdef USE_TEXT_ARCHIVE
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#else
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#endif
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

//
#define NUM_ITEMS       5
#define NUM_SUBITEMS    3
#define RANDOM_LEMGTH   8

//
void usage( char* argv )
{
    std::cerr << "Usage: " << argv << " [ load <archive-file> ] | [ save <archive-file> ]" << std::endl;
    exit( 1 );
}

//
std::string random_string()
{
    std::string chr = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    //
    while ( chr.size() != RANDOM_LEMGTH )
    {
        int pos = ( ( rand() % ( chr.size() - 1 ) ) );
        chr.erase( pos, 1 );
    }

    //
    return chr;
}

//
class arc
{
    public:
        //
        arc() {}

        //
        template <typename T>
        bool write( T& t, std::string f )
        {
            try
            {
                std::stringstream ss;
                #ifdef USE_TEXT_ARCHIVE
                boost::archive::text_oarchive out( ss, boost::archive::no_header );
                #else
                boost::archive::binary_oarchive out( ss, boost::archive::no_header );
                #endif

                out << t;

                //
                if ( ss.str().length() > 0 )
                {
                    std::ofstream ofs( f.c_str() );

                    if ( ofs.is_open() )
                    {
                        ofs << ss.str();
                        std::cout << ss.str().length() << " byte(s) written" << std::endl;
                    }
                    else
                    {
                        std::cerr << "Could not open archive file for writing" << std::endl;
                        return false;
                    }
                }
                else
                {
                    std::cerr << "Write stream is empty" << std::endl;
                    return false;
                }
            }
            catch ( std::exception& e )
            {
                std::cerr << "arc().write error: " << e.what() << std::endl;
                return false;
            }
            catch ( ... )
            {
                std::cerr << "arc().write: Something went wrong!" << std::endl;
                return false;
            }

            //
            return true;
        }

        //
        template <typename T>
        bool read( T& t, std::string f )
        {
            try
            {
                std::ifstream ifs( f.c_str() );

                //
                if ( ifs.is_open() )
                {
                    std::stringstream ss;

                    ss << ifs.rdbuf();

                    //
                    if ( ss.str().length() > 0 )
                    {
                        #ifdef USE_TEXT_ARCHIVE
                        boost::archive::text_iarchive in( ss, boost::archive::no_header );
                        #else
                        boost::archive::binary_iarchive in( ss, boost::archive::no_header );
                        #endif

                        in >> t;

                        std::cout << ss.str().length() << " byte(s) read" << std::endl;
                    }
                    else
                    {
                        std::cerr << "Read stream is empty" << std::endl;
                        return false;
                    }
                }
                else
                {
                    std::cerr << "Could not open archive file for reading" << std::endl;
                    return false;
                }
            }
            catch ( std::exception& e )
            {
                std::cerr << "arc().read error: " << e.what() << std::endl;
                return false;
            }
            catch ( ... )
            {
                std::cerr << "arc().read: Something went wrong!" << std::endl;
                return false;
            }

            return true;
        }

    protected:
    private:
};

//
class item
{
    public:
        //
        struct subitem
        {
            std::string name;
            std::string value;

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
            }
        };

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

//
void populate_item( item& itm )
{
    // create some dummy data
    itm.set_name( random_string() );

    for ( int i = 0; i < NUM_SUBITEMS; ++i )
        itm.get_subitems().push_back( item::subitem( random_string(),
                                                     random_string()
                                                   + "\n          "
                                                   + random_string() ) );
}

//
void populate_items( items& itm )
{
    // create more dummy data
    for ( int i = 0; i < NUM_ITEMS; ++i )
    {
        item val;

        populate_item( val );
        itm.push_back( val );
    }
}

//
void print_item( item& itm )
{
    std::cout << "name:     "  << itm.get_name() << std::endl;
    std::cout << "subitems  [" << itm.get_subitems().size() << "]" << std::endl;

    for ( item::subitems::iterator i = itm.get_subitems().begin(); i != itm.get_subitems().end(); ++i )
    {
        std::cout << "   name:  " << (*i).name  << std::endl;
        std::cout << "   value: " << (*i).value << std::endl;
    }

    std::cout << std::endl << std::flush; 
}

//
void print_items( items& itm )
{
    for ( items::iterator i = itm.begin(); i != itm.end(); ++i )
        print_item( (*i) );

    std::cout << itm.size() << " item(s)" << std::endl;
}

#ifndef TEST_TEMPLATES
void load_archive( items& itm, std::string file )
{
    std::ifstream ifs( file.c_str() );
    #ifdef USE_TEXT_ARCHIVE
    boost::archive::text_iarchive in( ifs, boost::archive::no_header );
    #else
    boost::archive::binary_iarchive in( ifs, boost::archive::no_header );
    #endif

    in >> itm;
}

//
void save_archive( items& itm, std::string file )
{
    std::ofstream ofs( file.c_str() );
    #ifdef USE_TEXT_ARCHIVE
    boost::archive::text_oarchive out( ofs, boost::archive::no_header );
    #else
    boost::archive::binary_oarchive out( ofs, boost::archive::no_header );
    #endif

    out << itm;
}
#endif // TEST_TEMPLATES

#ifdef TEST_TEMPLATES
//
template <typename T>
void write_archive( T& itm, std::string file )
{
    if ( ! arc().write<T>( itm, file ) )
        std::cerr << "write template failed" << std::endl;
}

//
template <typename T>
void read_archive( T& itm, std::string file )
{
    if ( ! arc().read<T>( itm, file ) )
        std::cerr << "read template failed" << std::endl;
}
#endif // TEST_TEMPLATES

//
int main( int argc, char** argv )
{
    std::srand( std::time( NULL ) );

    //
    if ( argc == 3 )
    {
        items itms;

        //
        if ( std::string( argv[ 1 ] ) == "load" )
        {
            #ifdef TEST_TEMPLATES
            read_archive<items>( itms, argv[ 2 ] );
            #else
            load_archive( itms, argv[ 2 ] );
            #endif
        }
        else
        {
            //
            if ( std::string( argv[ 1 ] ) == "save" )
            {
                populate_items( itms );

                #ifdef TEST_TEMPLATES
                write_archive<items>( itms, argv[ 2 ] );
                #else
                save_archive( itms, argv[ 2 ] );
                #endif
            }
            else
                usage( argv[ 0 ] );
        }

        print_items( itms );
    }
    else
        usage( argv[ 0 ] );

    return 0;
}

// use "make clean && make run" to test

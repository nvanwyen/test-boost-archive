// test POC for boost serialziation

//
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include "arc.hpp"
#include "item.hpp"

#define TEST_TEMPLATES

//
#define NUM_ITEMS       5
#define NUM_SUBITEMS    3
#define NUM_THINGS      9

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
void popluate_things( things& itm )
{
    for ( int i = 0; i < NUM_THINGS; ++i )
    {
        thing thg( random_string() );

        for ( int j; i < NUM_THINGS; ++i )
            thg.add_stuff( random_string() );

        itm.push_back( thg );
    }
}

//
void populate_item( item& itm )
{
    for ( int i = 0; i < NUM_SUBITEMS; ++i )
    {
        item::subitem sub( random_string(),
                           random_string()
                         + "\n          "
                         + random_string() );

        popluate_things( sub.other );
        itm.get_subitems().push_back( sub );
    }
}

//
void populate_items( items& itm )
{
    // create more dummy data
    for ( int i = 0; i < NUM_ITEMS; ++i )
    {
        item val( random_string() );

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

        for ( things::iterator t = (*i).other.begin(); t != (*i).other.end(); ++t )
        {
            std::cout << "    more: " << (*t).get_more() << std::endl;

            for ( std::vector<std::string>::iterator s = (*t).get_stuff().begin(); 
                  s != (*t).get_stuff().end(); ++s )
                std::cout << "       +: " << (*s) << std::endl;
        }
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

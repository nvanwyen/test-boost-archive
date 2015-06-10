#ifndef __ARC_HPP
#define __ARC_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <exception>

#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

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
                boost::archive::binary_oarchive out( ss, boost::archive::no_header );

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
                        boost::archive::binary_iarchive in( ss, boost::archive::no_header );

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

#endif // __ARC_HPP

#ifndef ITEMTRAITS_HEADER
#define ITEMTRAITS_HEADER

#include <new>
#include <string>
#include <iostream>
#include <cmath>
#include <limits>
#include <algorithm>
#include <type_traits>
#include "defs.h"

union TraitData {
    public:
    int     ival;
    double  dval;
    std::string sval;

    TraitData() {}
    ~TraitData() { }
    TraitData(const TraitData& UNUSED(pCopy)) {  }
};

class Trait {
    public:
        //Mode types
        enum {
            NoMode = 0,
            IntMode,
            DoubleMode,
            StringMode
        };

    public:
        int Mode = NoMode;
        TraitData my_data = TraitData();

    public:
        Trait() { }
        Trait(const int& pInt) : Mode(IntMode) { new(&my_data.ival) int(pInt); }
        Trait(const double& pDouble) : Mode(DoubleMode) { new(&my_data.dval) double(pDouble); }
        Trait(const std::string& pString) : Mode(StringMode) { new(&my_data.sval) std::string(pString); }
        Trait(int&& pInt) : Mode(IntMode) { new(&my_data.ival) int(pInt); }
        Trait(const char* pCString) : Mode(StringMode) { new(&my_data.sval) std::string(pCString); }
        Trait(double&& pDouble) : Mode(DoubleMode) { new(&my_data.dval) double(std::move(pDouble)); }
        Trait(std::string&& pString) : Mode(StringMode) { new(&my_data.sval) std::string(std::move(pString)); }

        //Copy ctor
        Trait(const Trait& pCopy)
        {
            if(Mode !=  Trait::NoMode){ //No conversion available or not necessary
                return;
            }else if(Mode == pCopy.Mode){
                if(Mode == IntMode)
                    my_data.ival = pCopy.my_data.ival;
                else if(Mode == DoubleMode)
                    my_data.dval = pCopy.my_data.dval;
                else if(Mode == StringMode)
                    my_data.sval = pCopy.my_data.sval;

                return;
            }

            Mode = pCopy.Mode;

            switch(Mode){
                default:
                    break;
                case IntMode:
                    new(&my_data.ival) int(pCopy.my_data.ival);
                    break;
                case DoubleMode:
                    new(&my_data.dval) double(pCopy.my_data.dval);
                    break;
                case StringMode:
                    new(&my_data.sval) std::string(pCopy.my_data.sval);
                    break;
            };
        }

        //Move ctor
        Trait(Trait&& pMove)
        {
            if(pMove.Mode ==  Trait::NoMode){ //No conversion available or not necessary
                return;
            }else if(Mode == pMove.Mode){
                if(Mode == IntMode)
                    my_data.ival = std::move(pMove.my_data.ival);
                else if(Mode == DoubleMode)
                    my_data.dval = std::move(pMove.my_data.dval);
                else if(Mode == StringMode)
                    my_data.sval = std::move(pMove.my_data.sval);

                return;
            }

            Mode = std::move(pMove.Mode);

            switch(Mode){
                default:
                    break;
                case IntMode:
                    new(&my_data.ival) int(std::move(pMove.my_data.ival));
                    break;
                case DoubleMode:
                    new(&my_data.dval) double(std::move(pMove.my_data.dval));
                    break;
                case StringMode:
                    new(&my_data.sval) std::string(std::move(pMove.my_data.sval));
                    break;
            };
        }

        ~Trait()
        {
            using std::string;
            if(Mode == StringMode) {
                my_data.sval.~string();
            }
            Mode = NoMode;
        }

    public:
        Trait& operator=(const Trait& pCopy)
        {
            if(pCopy.Mode == Trait::NoMode){
                return *this;
            } else if(Mode == pCopy.Mode){
                if(Mode == Trait::IntMode){
                    my_data.ival = pCopy.my_data.ival;
                } else if(Mode == Trait::DoubleMode){
                    my_data.dval = pCopy.my_data.dval;
                } else if(Mode == Trait::StringMode){
                    my_data.sval = pCopy.my_data.sval;
                }

                return *this;
            }

            Mode = pCopy.Mode;

            switch(Mode){
                default:
                    return *this;
                case IntMode:
                    my_data.ival = pCopy.my_data.ival;
                    return *this;
                case DoubleMode:
                    my_data.dval = pCopy.my_data.dval;
                    return *this;
                case StringMode:
                    new(&my_data.sval) std::string(pCopy.my_data.sval);
                    //my_data.sval = pCopy.my_data.sval;
                    return *this;
            };

            return *this;
        }

        Trait& operator=(Trait&& pMove)
        {
            if(pMove.Mode == Trait::NoMode){
                return *this;
            } else if(Mode == pMove.Mode){
                if(Mode == Trait::IntMode){
                    my_data.ival = std::move(pMove.my_data.ival);
                } else if(Mode == Trait::DoubleMode){
                    my_data.dval = std::move(pMove.my_data.dval);
                } else if(Mode == Trait::StringMode){
                    my_data.sval = std::move(pMove.my_data.sval);
                }
                return *this;
            } else {
                if(Mode == StringMode){
                    using std::string;
                    my_data.sval.~string();
                }
            }

            Mode = std::move(pMove.Mode);

            switch(Mode){
                default:
                    break;
                case IntMode:
                    new(&my_data.ival)  int(std::move(pMove.my_data.ival));
                    break;
                case DoubleMode:
                    //std::cout << "Init double to " << pMove.my_data.dval << std::endl;;
                    my_data.dval = std::move(pMove.my_data.dval);
                    //new(&my_data.dval)  double(std::move(pMove.my_data.dval));
                    break;
                case StringMode:
                    new(&my_data.sval)  std::string(std::move(pMove.my_data.sval));
                    break;
            }

            return *this;
        }


        Trait& operator=(const int& pInt)
        {
            if(Mode == StringMode){
                using std::string;
                my_data.sval.~string();
            }
            Mode = IntMode;
            my_data.ival = pInt;
            return *this;
        }
     //   Trait& operator=(const int&& pInt) { Mode = IntMode; my_data.ival = pInt; return *this; }
        Trait& operator=(const double& pDbl)
        {
            if(Mode == StringMode) { using std::string; my_data.sval.~string(); }
            Mode = DoubleMode; my_data.dval = pDbl; return *this;
        }
    //    Trait& operator=(const double&& pDbl) { Mode = DoubleMode; my_data.dval = pDbl; return *this; }
        Trait& operator=(const std::string& pString)
        {
            if(Mode != StringMode){
                Mode = StringMode;
                new(&my_data.sval) std::string(pString);
                return *this;
            }

            my_data.sval = pString;
            return *this;
        }

        Trait& operator=(const char* pCString)
        {
            if(Mode != StringMode)
            {
                Mode = StringMode;
                new(&my_data.sval) std::string(pCString);
                return *this;
            }

            my_data.sval = pCString;
            return *this;
        }

        bool operator!=(const Trait& pCompare) const
        {
            return !(operator==(pCompare));
        }

        bool operator==(const Trait& pCompare) const
        {
            if(pCompare.Mode != Mode){
                return false;
            }
            switch(Mode){
                default:
                    return false;
                case NoMode:
                    return false;
                case IntMode:
                    return pCompare.my_data.ival == my_data.ival;
                case DoubleMode:
                    return  ((my_data.dval - pCompare.my_data.dval) < 2.0*std::numeric_limits<double>::epsilon());
                case StringMode:
                    return pCompare.my_data.sval == my_data.sval;
            };

            return false;
        }

        std::string ToString(void) const
        {
            if(Mode == NoMode)
                return "__NOVALUE__";
            else if(Mode == IntMode)
                return std::to_string(my_data.ival);
            else if(Mode == DoubleMode)
                return std::to_string(my_data.dval);
            else if(Mode == StringMode)
                return my_data.sval;

            return "__NOVALUE__";
        }
        //Comparison
        inline bool operator==(const int& pIcmp){ return (Mode == IntMode) ? (my_data.ival == pIcmp) : false; }
        inline bool operator==(const double& pDcmp){ return (Mode == DoubleMode) ? (my_data.dval == pDcmp) : false; }
        inline bool operator==(const std::string& pScmp){ return (Mode == StringMode) ? (my_data.sval == pScmp) : false; }
        inline bool operator==(const char* pCString)
        {
            return (Mode == StringMode) ? (my_data.sval == pCString) : false;
        }

        inline bool is_numeric(void) const
        {
            /*return (Mode == StringMode) && !my_data.sval.empty() &&
                    std::find_if(begin(my_data.sval), end(my_data.sval), [](char c) ->bool{ return !std::isdigit(c); }) == end(my_data.sval);*/
            return false;
        }

        template<class T> T& Ref(void);
        template<class T> const T Convert(void) const;
};




#endif //#define ITEMTRAITS_HEADER

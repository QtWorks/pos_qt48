#ifndef PRINT_H
#define PRINT_H

#include <cups/cups.h>
#include "generics.h"
#include <utility>
#include <cstdint>
#include <memory>
#include <vector>
#include <sstream>

class Sale;

class PrinterHandler {
    //TOTO: implement a queue;
    std::ostringstream postscript;
    std::string ps_functions;   //!< Functions text appended to every file
    std::string ps_image;    //!< Image definition text used if print_image = true;
    std::string ps_name;        //!< Bold faced name text for every page
    std::string ps_info;        //!< Info text. Changes a lot, depending on what is printed.
    std::string ps_items;       //!< Items rung up;
    std::string ps_footer;      //!< Footer text
    std::string ps_header;      //!< Header. phone #, adress

    int         printer_count   = 0;
    cups_dest_t *printer_list   = nullptr;
    cups_dest_t *receipt        = nullptr;
    cups_dest_t *invoice        = nullptr;
    cups_dest_t *order          = nullptr;

    void generate_postscript();
    int  print_postscript();

    std::string int2hex(const u_int8_t&);
    std::string scanline_to_hex(const unsigned char* line, int len, int bytes);
public:
    bool print_image        = false;
    bool custom_footer      = false;
    bool print_header       = false;
    bool print_tableinfo    = false;

    PrinterHandler();
    virtual ~PrinterHandler();

    /// @brief  print anything
    template <typename T>
    int print(T);
    int print_order(Sale* sale);

    void set_name_text(const std::string&);
    void set_header_text(const std::string&);
    int  set_label_image(const std::string&);
    void set_footer_text(const std::string&);
};

typedef GenericSingleton<PrinterHandler> Printer;

#endif // PRINT_H

#include "print.h"
#include "sale.h"
#include "user.h"
#include "defs.h"
#include "ordereditem.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <QImage>
#include <QFile>

PrinterHandler::~PrinterHandler()
{
    if( printer_list ) {
        //cupsFreeDests( printer_count, printer_list );
    }
}

PrinterHandler::PrinterHandler()
{
    printer_count = cupsGetDests(&printer_list);
    receipt = printer_list++;
    order = receipt;

    std::ostringstream oss;

    oss << "/textbox {/lm 12 def /bm 0 def /rm 10 def /tm 640 def lm tm tm } def\n";
    oss << "/newline { tm 12 sub /tm exch def lm tm moveto } def\n";
    oss << "/pagewidth { initclip clippath flattenpath pathbbox pop exch pop exch pop } def\n";
    oss << "/center { dup stringwidth pop 2 div pagewidth 2 div sub neg tm moveto } def\n";
    oss << "/right { dup stringwidth pop pagewidth exch sub rm sub tm moveto } def\n";
    oss << "/left { lm tm moveto } def\n";
    oss << "/halfleft { lm 2 div tm moveto } def\n";
    oss << "/item { left } def\n/subitem { lm 2 mul tm moveto } def\n";
    oss << "/header_font { /Helvetica-Bold findfont 12 scalefont setfont } def\n";
    oss << "/info_font {/Helvetica findfont 7 scalefont setfont } def\n";
    oss << "/item_font {/Helvetica findfont 10 scalefont setfont } def\n";
    oss << "/order_font {/Helvetica findfont 13 scalefont setfont } def\n";
    oss << "/total_font {/Helvetica-Bold findfont 10 scalefont setfont } def\n";
    oss << "/subitem_font {/Helvetica findfont 8 scalefont setfont } def\n";
    oss << "/totalr { dup stringwidth pop pagewidth exch sub rm 4 mul sub tm moveto } def\n";

    ps_functions = oss.str();

    oss.str("");
    oss << "textbox\n";
    oss << "header_font newline (ROK KOREAN BBQ) center show\n";
    oss << "item_font\n";
    oss << "newline (\\(954\\) 530 - 1394) center show\n";
    oss << "newline (4954 N University Dr, Lauderhill) center show\n";
    oss << "newline (FL 33319) center show\n";
    oss << "newline\n";

    ps_header = oss.str();
    oss.str("");

    set_label_image("/home/art/rok.png");
    print_image = true;
}

void PrinterHandler::generate_postscript()
{
    postscript.str("");

    postscript << "%!PS\n";
    postscript << ps_functions;

    if( print_image )       postscript << ps_image;
    if( print_header )      postscript << ps_header;

    postscript << ps_items;
    postscript << "showpage\n";
    postscript << "%%EOF";
}

int PrinterHandler::print_postscript()
{
    cups_option_t *options = nullptr;
    int num_options = 0;
    int job_id = cupsCreateJob(CUPS_HTTP_DEFAULT, receipt->name, "receipt", num_options, options);
    if( !job_id ) {
        std::cout << cupsLastErrorString() << std::endl;
        return -1;
    }
    cupsStartDocument( CUPS_HTTP_DEFAULT, receipt->name, job_id, "SaleReceipt", CUPS_FORMAT_POSTSCRIPT, 1);
    cupsWriteRequestData( CUPS_HTTP_DEFAULT, postscript.str().c_str(), postscript.str().size() );
    cupsFinishDocument( CUPS_HTTP_DEFAULT, receipt->name );

    return 0;
}

std::string PrinterHandler::int2hex(const u_int8_t& val) {
    const char hex[] = "0123456789ABCDEF";
    std::ostringstream ret;
    ret << hex[(val & 0xF0) >> 4] << hex[val & 0x0F];
    return ret.str();
}

std::string PrinterHandler::scanline_to_hex(const unsigned char* line, int len, int bytes)
{
    std::string ret;
    for( int i = 0; i < len * bytes; i+= 3 ) {
        ret += int2hex(line[i]);
        ret += int2hex(line[i + 1]);
        ret += int2hex(line[i + 2]);
    }

    return ret;
}

/// @note   gsave
///         pagewidth 2 div 100 sub 600 translate %pre-defined constants
///         200 -180 scale  %predefined constant values
///         IMG_WIDTH IMG_HEIGHT bits [IMG_WIDTH 0 0 -IMG_HEIGHT 0 1]
///         {< ... >}
///         false 3 colorimage
int PrinterHandler::set_label_image(const std::string& filepath)
{
    QImage img;
    if( img.load( filepath.c_str() ) ){
        img = img.convertToFormat(QImage::Format_RGB888);

        std::ostringstream ss;

        int width = img.width();
        int height = img.height();
        ss << "gsave\n";
        ss << "pagewidth 2 div 100 sub 600 translate\n";
        //ss << 20 << " " << scaled.height() << " translate\n";
        ss << 200 << " -" << 180 << " scale\n";
        ss << width << " " << height << " 8 " << "[" << width << " 0 0 -" << height << " 0 1]\n";
        ss << "{<\n";

        for( int y = img.height() - 1; y > -1; y-- ) {
            auto scanline = scanline_to_hex(img.scanLine(y), img.width(), 3);
            ss << scanline << "\n";
        }
        ss << ">}\n";
        ss << "false 3 colorimage\n";
        ss << "grestore\n";

        ps_image = ss.str();
        ss.str("");

        return 0;
    } else {
        return -1;
    }

    return -2;
}

template<typename T> int PrinterHandler::print(T UNUSED(t))
{
    return -1;
}

template<> int PrinterHandler::print(Sale* sale)
{
    std::ostringstream oss;
    oss << "info_font\n";
    oss << "newline (ORDER #: " << sale->id << ") left show (Server: " <<  sale->user->first_name << ") right show\n";
    oss << "item_font\n";
    oss << "newline\n";
    oss << std::setprecision(2) << std::fixed;

    for( const auto &item : sale->get_items() ) {
        if( item->subitem ) {
            oss << "subitem_font\n";
            oss << "newline (" << item->name << ") subitem show (" << item->total << ") right show\n";
            oss << "item_font\n";
        }
        else {
            oss << "newline (" << item->name << ") left show (" << item->total << ") right show\n";
        }
    }

    oss << "newline\n";
    oss << "newline (SUBTOTAL) totalr show (" << sale->sub_total << ") right show\n";
    oss << "newline (TAX) totalr show (" << sale->tax << ") right show\n";
    oss << "total_font newline (TOTAL) totalr show (" << sale->total << ") right show\n";
    oss << "newline\n";
    oss << "newline (Thank You!) center show\n";
    ps_items = oss.str();

    generate_postscript();

    cups_option_t *options = nullptr;
    int num_options = 0;
    int job_id = cupsCreateJob(CUPS_HTTP_DEFAULT, receipt->name, "receipt", num_options, options);
    if( !job_id ) {
        std::cout << cupsLastErrorString() << std::endl;
        return -1;
    }
/*
    QFile file("/home/art/save.ps");
    file.open(QFile::WriteOnly | QFile::Text );
    file.write(postscript.str().c_str(), postscript.str().size() );
    file.close();
*/
    cupsStartDocument( CUPS_HTTP_DEFAULT, receipt->name, job_id, "SaleReceipt", CUPS_FORMAT_POSTSCRIPT, 1);
    cupsWriteRequestData( CUPS_HTTP_DEFAULT, postscript.str().c_str(), postscript.str().size() );
    cupsFinishDocument( CUPS_HTTP_DEFAULT, receipt->name );

    return 0;
}

int PrinterHandler::print_order(Sale* sale)
{
    std::ostringstream oss;
    print_image = false;
    print_header = false;

    oss << "textbox\n";
    oss << "order_font\n";
    oss << "newline (ORDER #: " << sale->id << ") center show\n";
    oss << "newline\n";
    oss << "newline\n";
    oss << "newline\n";
    oss << "newline\n";
    oss << "newline\n";
    oss << "newline (SERVER #: " << sale->user->first_name << ") left show (TABLE#: " << (sale->table_number + 1) << ") right show\n";
    oss << "newline\n";

    int item_count = 0;
    for( const auto item : sale->get_unsaved_items() ) {
        if( item->subitem ) {
            oss << "item_font\n";
            oss << "newline (" << item->name << ") subitem show\n";
            oss << "order_font\n";
        }
        else {
            oss << "newline\n";
            oss << "newline (" << item->name << ") left show\n";
            item_count++;
        }
    }

    oss << "newline\n";
    oss << "newline\n";
    oss << "newline\n";
    oss << "header_font\n";
    oss << "newline (ITEM #: " << item_count << ") center show\n";

    ps_items = oss.str();

    generate_postscript();

    QFile file("/home/art/save.ps");
    file.open(QFile::WriteOnly | QFile::Text );
    file.write(postscript.str().c_str(), postscript.str().size() );
    file.close();

    return 0;//print_postscript();
}

void PrinterHandler::set_header_text(const std::string& header)
{
}

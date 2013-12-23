.pragma library

var api = undefined;

var get_allyears;
var get_year;
var get_month;
var get_day;
var get_sale;
var get_today;
var report_date_stats;
var print_sale;

var saleReportModel;
var saleOrderModel;
var employeeSaleModel;
var itemDetailSaleModel;
var hourDetailSaleModel;

function init( _api_ ) {
    api = _api_;

    get_allyears    = api.get_allyears;
    get_year        = api.get_year;
    get_month       = api.get_month;
    get_day         = api.get_day;
    get_sale        = api.get_sale;
    get_today       = api.get_today;
    report_date_stats = api.report_date_stats;
    print_sale      = api.print_sale;

    saleReportModel = api.saleReportModel;
    saleOrderModel  = api.saleOrderModel;
    employeeSaleModel = api.employeeSaleModel;
    itemDetailSaleModel = api.itemDetailSaleModel;
    hourDetailSaleModel = api.hourDetailSaleModel;
}


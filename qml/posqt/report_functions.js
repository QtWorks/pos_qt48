
function report_all_years()
{
    Reports.get_allyears();
    Reports.report_date_stats(-1, -1, -1);
}

function report_today()
{
    select_report( today_btn );
    show_report_list( true );
    report_by_item( false );
    var date = new Date();
    Reports.get_today();
    Reports.report_date_stats( date.getFullYear(), date.getMonth() + 1, date.getDate() );
    selected = "Today, " + Qt.formatDateTime(date, "(MM.dd.yyyy)");
}

function report_this_year()
{
    select_report( year_btn );
    show_report_list( true );
    //reportBox.show();
    var date = new Date();
    Reports.get_year( date.getFullYear() );
    Reports.report_date_stats( date.getFullYear(), -1, -1 );
    selected = "Year " + date.getFullYear();
}

function report_this_month()
{
    select_report( month_btn );
    show_report_list( true );
    var date = new Date();
    Reports.get_month( date.getFullYear(), date.getMonth() + 1 );
    Reports.report_date_stats( date.getFullYear(), date.getMonth() + 1, -1 );
    selected = App.months[date.getMonth()] + " of " + date.getFullYear();
}

function report_year( yr )
{
    Reports.get_year( yr );
    Reports.report_date_stats( yr, -1, -1 );
}

function report_month( yr, mo )
{
    Reports.get_month( yr, mo );
    Reports.report_date_stats( yr, mo, -1 );
}

function report_date( yr, mo, dy )
{
    Reports.get_day( yr, mo, dy );
    Reports.report_date_stats( yr, mo, dy );
}

function report_item_click( id, lbl, ttl, sc, yr, mo, dy )
{
    total = "Total: " + ttl.toFixed(2);
    sale_count = "Number of Sales : " + sc;

    switch( lbl ) {
        case "year":
            year = yr;
            selected = "Year " + yr;
            report_year( yr );
            break;
        case "month":
            selected = App.months[mo - 1] + " of " + yr;
            month = mo;
            report_month( yr, mo );
            break;
        case "day":
            selected = dy + ", " + selected;
            day = dy;
            report_date( yr, mo, dy );
            break;
        case "Back to Year" :
            selected = "YEAR OF " + yr;
            total = "";
            sale_count = "";
            month = -1;
            report_year( yr );
            break;
        case "Back to Month" :
            selected = App.months[mo - 1] + " of " + yr;
            total = "";
            sale_count = "";
            day = -1;
            report_month( yr, mo );
            break;
        case "Back to All" :
            selected = "ALL YEARS";
            total = "";
            year = -1;
            report_all_years();
            break;
        case "sale":
            displaySale( id, ttl );
    }
}

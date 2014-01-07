import QtQuick 1.1;
import "reports.js" as Reports;
import "colors.js" as Colors;
import "app.js" as App;
import "menu.js" as MenuLib;
import "menu_functions.js" as MenuMethods;
import "report_functions.js" as ReportMethods;

Item {
    anchors.fill: parent;
    property string selected : "None";
    property string total : "";
    property string sale_count : "";
    property variant catModel : MenuLib.get_catModel();
    property int    current_cat : -1;
    property variant selected_btn : undefined;
    property int year : -1;
    property int month : -1;
    property int day : -1;

    function report_item_click(item_id, item_label, item_total,
                                item_salecount, item_year, item_month, item_day )
    { ReportMethods.report_item_click(item_id, item_label, item_total, item_salecount, item_year, item_month, item_day); }

    function displaySale( iid, ttl ) {
        ticket.ticketSubTotal = ttl - (ttl / 106 * 6);
        Reports.get_sale( iid );
        ticket.userName = "Server : " + Reports.api.sale_username;
        quickview.visible = true;
    }

    function report_by_item( opt ) {
        if( opt ) {
            report_list.orientation = ListView.Horizontal;
            report_list.delegate = null; //This line supresses erros when switching models
            report_list.model = catModel;
            report_list.delegate = menuCatDelegate;
        }
        else {
            report_list.orientation = ListView.Vertical;
            report_list.delegate = null;
            report_list.model = Reports.saleReportModel;
            report_list.delegate = reportDelegate;
        }
    }

    function show_report_list( opt ) {
        if( opt ) {//&& !reportBox.visible ) {
            reportBox.show();
        }
        else {
            reportBox.hide();
            if( selected_btn ) {
                //selected_btn.height = selected_btn.height/2;//200;
                //selected_btn.width = selected_btn.width/2;
            }
        }
    }

    function select_report( btn ) {
        //btn.height = btn.height * 2;
        //btn.width = btn.width * 2;
        selected_btn = btn;
        reportBox.color = Colors.make( btn.color, "dd" );
        report_icon.source = btn.icon;
    }

    Component {
        id: reportDelegate;
        ReportItem
        {
            height: list_bg.height * 0.15;
            width: report_list.width;
        }
    }

    Rectangle {
        id: bbox;
        anchors.right: parent.right;
        anchors.left: parent.left;
        height: parent.height * 0.5;
        width: parent.height * 0.5;
        anchors.verticalCenter: parent.verticalCenter;
        anchors.leftMargin: 1;
        anchors.rightMargin: 1;
        color: "black";

        Image {
            anchors.left: parent.left; anchors.leftMargin: sourceSize.width * 0.3;
            anchors.verticalCenter: parent.verticalCenter;
            source: "reports_ico3.svg";
            sourceSize.width: parent.height * 0.5;
            sourceSize.height: parent.height * 0.5;
        }
        Grid {
            id: reportButtonRow;
            anchors.centerIn: parent;
            spacing: 10;
            columns: 4;
            rows: 2;
            Button {
                id: timeclock_btn;
                width: bbox.height * 0.5 - 10;
                height: bbox.height * 0.5 - 10;
                color: Colors.blue3;
                icon: "clock_ico.svg";
                pixelSize: height * 0.1;
                label: "Timeclock";
                font: "Chunkfive";
                onButtonClick : {
                    App.load_subview("ReportsTimecardView.qml");
                }
            }

            Button {
                id: inv_btn;
                width: bbox.height * 0.5 - 10;
                height: bbox.height * 0.5 - 10;
                color: Colors.orange;
                label: "Inventory";
                icon: "inventory_ico.svg";
                pixelSize: height * 0.1;
                font: "Chunkfive";
                onButtonClick : {
                    select_report( inv_btn );
                    show_report_list( true );
                }
            }

            Button {
                id: all_btn;
                width: bbox.height * 0.5 - 10;
                height: bbox.height * 0.5 - 10;
                icon: "infinity_ico.svg";
                color: Colors.main_blue;
                pixelSize: height * 0.1;
                font: "Chunkfive";
                label: "All";
                onButtonClick : {
                    select_report( all_btn );
                    show_report_list( true );
                    report_by_item( false );
                    ReportMethods.report_all_years();
                    selected = "ALL YEARS";
                }
            }
            Button {
                id: today_btn;
                width: bbox.height * 0.5 - 10;
                height: bbox.height * 0.5 - 10;
                icon: "calendar_ico.svg";
                pixelSize: height * 0.1;
                font: "Chunkfive";
                label: "Today";
                color: Colors.blue6;
                onButtonClick : ReportMethods.report_today();
            }
            Button {
                id: money_btn;
                width: bbox.height * 0.5 - 10;
                height: bbox.height * 0.5 - 10;
                color: Colors.yellow3;
                label: "Employee";
                icon: "hardhat_ico.svg";
                font: "Chunkfive";
                pixelSize: height * 0.1;
                onButtonClick : {
                    select_report( money_btn );
                    show_report_list( true );
                }
            }
            Button {
                id: payments_btn;
                width: bbox.height * 0.5 - 10;
                height: bbox.height * 0.5 - 10;
                color: Colors.green3;
                label: "Payments";
                icon: "tag_ico.svg";
                font: "Chunkfive";
                pixelSize: height * 0.1;

            }

            Button {
                id: month_btn;
                width: bbox.height * 0.5 - 10;
                height: bbox.height * 0.5 - 10;
                pixelSize: height * 0.1;
                icon: "calendar_m_ico.svg";
                label: "Month";
                font: "Chunkfive";
                color: Colors.marine_violet;
                onButtonClick : ReportMethods.report_this_month();
            }

            Button {
                id: year_btn;
                height: bbox.height * 0.5 - 10;
                width: bbox.height * 0.5 - 10;
                pixelSize: height * 0.1;
                icon: "calendar_y_ico.svg";
                font: "Chunkfive";
                label: "Year";
                color: Colors.violet3;
                onButtonClick : ReportMethods.report_this_year();
            }
        }
    }
    FadeView {
        id: reportBox;
        anchors.fill: parent;
        color: Colors.make( Colors.grey2, "aa" );
        dim: true;
        dimColor: Colors.make( "#000000", "dd" );

        onHalfShown : {
            App.admin_console_set_interactive( false );
            detail_slide.show();
        }

        onHidden : {
            App.admin_console_set_interactive( true );
        }

        onClicked : {
            detail_slide.hide();
            user_detail_slide.hide();
            item_detail_slide.hide();
            show_report_list( false );
        }

        Image {
            id: report_icon;
            sourceSize.height: parent.height * 0.2;
            sourceSize.width: parent.height * 0.2;
            anchors.right: parent.right; anchors.rightMargin: parent.width * 0.1;
            anchors.top: parent.top;
        }

        Text {
            id: total_text;
            anchors.left: white_bar.left; anchors.leftMargin: parent.width * 0.01;
            anchors.top: white_bar.bottom; anchors.topMargin: parent.height * 0.003;
            font.family: "Molot";
            font.wordSpacing: -10;
            font.pixelSize: App.screen_h * 0.055;
            text: "Total:";
            color: Colors.make( "#ffffff", "cc" );
        }
        Text {
            id: sale_count_text;
            anchors.left: white_bar.left; anchors.leftMargin: parent.width * 0.01;
            anchors.top: total_text.bottom; anchors.topMargin: parent.height * 0.003;
            font.family: "Molot";
            font.pixelSize: App.screen_h * 0.05;
            text: "Cash Tendered:";// + Reports.api.dateSaleCount;//sale_count;
            color: "#99ffffff";
        }
        Text {
            id: date_cc_text;
            anchors.left: sale_count_text.left;
            anchors.top: sale_count_text.bottom; anchors.topMargin: parent.height * 0.003;
            font.family: "Molot";
            font.pixelSize: App.screen_h * 0.03;
            text: "Credit Total:";
            color: "#99ffffff";
        }
        Text {
            id: date_tender_text;
            anchors.left: date_cc_text.left;
            anchors.top: date_cc_text.bottom; anchors.topMargin: parent.height * 0.003;
            font.family: "Molot";
            font.pixelSize: App.screen_h * 0.03;
            text: "Sale Count:";
            color: "#99ffffff";
        }
        Text {
            anchors.right: list_bg.left; anchors.rightMargin: parent.width * 0.01;
            anchors.top: total_text.top;
            font.family: "Molot";
            font.pixelSize: App.screen_h * 0.055;
            text: Reports.api.dateTotal.toFixed(2);
            color: Colors.make( "#ffffff", "cc" );
        }
        Text {
            anchors.right: list_bg.left; anchors.rightMargin: parent.width * 0.01;
            anchors.top: sale_count_text.top;
            font.family: "Molot";
            font.pixelSize: App.screen_h * 0.05;
            text: Reports.api.dateTendered.toFixed(2);
            color: "#99ffffff";
        }
        Text {
            anchors.right: list_bg.left; anchors.rightMargin: parent.width * 0.01;
            anchors.top: sale_count_text.bottom; anchors.topMargin: parent.height * 0.003;
            font.family: "Molot";
            font.pixelSize: App.screen_h * 0.03;
            text: Reports.api.dateCreditTotal.toFixed(2);
            color: "#99ffffff";
        }
        Text {
            anchors.right: list_bg.left; anchors.rightMargin: parent.width * 0.01;
            anchors.top: date_cc_text.bottom; anchors.topMargin: parent.height * 0.003;
            font.family: "Molot";
            font.pixelSize: App.screen_h * 0.03;
            text: Reports.api.dateSaleCount;
            color: "#99ffffff";
        }

        Slide {
            id: user_detail_slide;
            anchors.bottom: list_bg.bottom;
            anchors.top: white_bar.bottom; anchors.topMargin: parent.height * -0.05;

            x: -width;
            property: "x";
            from: - width;
            to: 0;
            color: "#ef000000";
            width: parent.width;

            Image {
                id: user_ico;
                source: "hardhat_ico.svg";
                sourceSize.height: parent.height * 0.4;
                sourceSize.width: parent.height * 0.4;
                anchors.verticalCenter: parent.verticalCenter;
                anchors.left: parent.left; anchors.leftMargin: parent.width * 0.01;
            }
            Item {
                anchors.left: user_ico.right; anchors.leftMargin: parent.width * 0.05;
                anchors.right: parent.right; anchors.rightMargin: parent.width * 0.45;
                anchors.top: parent.top; anchors.topMargin: parent.height * 0.1;
                anchors.bottom: parent.bottom; anchors.bottomMargin: parent.height * 0.1;
                ReportsUserView {}
            }
        }
        Slide {
            id: item_detail_slide;
            anchors.bottom: list_bg.bottom;
            anchors.top: white_bar.bottom; anchors.topMargin: parent.height * -0.05;
            x: -width;
            property: "x";
            from: -width;
            to: 0;
            color: "#ef000000";
            width: parent.width;

            Image {
                id: item_ico;
                source: "barcode_ico.svg";
                sourceSize.height: parent.height * 0.4;
                sourceSize.width: parent.height * 0.4;
                anchors.verticalCenter: parent.verticalCenter;
                anchors.left: parent.left; anchors.leftMargin: parent.width * 0.01;
            }
            Item {
                anchors.left: item_ico.right; anchors.leftMargin: parent.width * 0.05;
                anchors.right: parent.right; anchors.rightMargin: parent.width * 0.45;
                anchors.top: parent.top; anchors.topMargin: parent.height * 0.1;
                anchors.bottom: parent.bottom; anchors.bottomMargin: parent.height * 0.1;
                ReportsItemView {}
            }
        }
        Slide {
            id: hour_detail_slide;
            anchors.bottom: list_bg.bottom;
            anchors.top: white_bar.bottom; anchors.topMargin: parent.height * -0.05;
            x: -width;
            property: "x";
            from: -width;
            to: 0;
            color: "#ef000000";
            width: parent.width;

            Image {
                id: clock_ico;
                source: "clock_ico.svg";
                sourceSize.height: parent.height * 0.4;
                sourceSize.width: parent.height * 0.4;
                anchors.verticalCenter: parent.verticalCenter;
                anchors.left: parent.left; anchors.leftMargin: parent.width * 0.01;
            }
            Item {
                anchors.left: clock_ico.right; anchors.leftMargin: parent.width * 0.05;
                anchors.right: parent.right; anchors.rightMargin: parent.width * 0.45;
                anchors.top: parent.top; anchors.topMargin: parent.height * 0.1;
                anchors.bottom: parent.bottom; anchors.bottomMargin: parent.height * 0.1;
                ReportsHourView {}
            }
        }
        Rectangle {
            id: list_bg;
            height: App.screen_h * 0.7;
            width: App.screen_w * 0.4;
            anchors.right: parent.right; anchors.rightMargin: parent.width * 0.01;
            anchors.top: selected_text.bottom;
            color: Colors.make( "#000000" , "00" );
            radius: 10;
            Behavior on y { NumberAnimation { duration: 500; easing.type: Easing.InOutCirc; } }

            ListView {
                id: report_list;
                spacing: 20;
                delegate: reportDelegate;
                anchors.fill: parent;
                anchors.topMargin: 20;
                //cacheBuffer: 1000;
                model: Reports.saleReportModel;
                clip: true;
                snapMode: ListView.SnapToItem;
                boundsBehavior: Flickable.StopAtBounds;

                /*displaced : Transition {
                    NumberAnimation { property: "y"; to: 0; duration: 100; }
                }

                add : Transition {
                    NumberAnimation { property: "y"; from: 0; duration: 200; }
                }*/
            }
        }

        Text {
            id: selected_text;
            anchors.left: parent.left; anchors.leftMargin: parent.width * 0.1;
            anchors.top: parent.top; anchors.topMargin: parent.height * 0.05;
            font.family: "Molot";
            font.wordSpacing: -10;
            font.pixelSize: App.screen_h * 0.09;
            text: selected;
            color: Colors.make( "#ffffff", "cc" );
        }

        Rectangle {
            id: white_bar;
            anchors.top: list_bg.top; anchors.topMargin: list_bg.height * 0.15 + 26;
            anchors.left: parent.left;
            anchors.right: list_bg.left;
            height: 4;
            color:  "white";
        }

        Slide {
            id: detail_slide;
            anchors.bottom: parent.bottom; anchors.bottomMargin: App.screen_h * 0.11;
            x: -width;
            to: 0;
            from: -width;
            property: "x";
            color: "#00000000";
            width: parent.width * 0.58;
            height: parent.height * 0.15;

            Button {
                id: detail_ico;
                color: "#00000000";
                icon: "zoom_ico.svg";
                width: parent.height * 0.9;
                height: parent.height * 0.9;
                anchors.left: parent.left; anchors.leftMargin: parent.width * 0.05;
                anchors.verticalCenter: parent.verticalCenter;

                onButtonClick : {
                    loader.source = "";
                    loader.source = "CashOutView.qml";
                    /*totals_report.total = Reports.api.dateTotal;
                    totals_report.tendered = Reports.api.dateTendered;
                    totals_report.credit = Reports.api.dateCreditTotal;
                    totals_report.cash = Reports.api.dateCashTotal;
                    */
                    loader.item.total = Reports.api.dateTotal;
                    loader.item.tendered = Reports.api.dateTendered;
                    loader.item.credit = Reports.api.dateCreditTotal;
                    loader.item.cash = Reports.api.dateCashTotal;

                    loader.item.dim = true;
                    loader.item.dimColor = "#99000000";
                    loader.item.show_expected = false;
                    loader.item.color = Colors.make( Colors.violet3, "cc" );
                    loader.item.show();
                    //totals_report.show();
                }
            }
            Row {
                anchors.left: detail_ico.left; anchors.leftMargin: parent.width * 0.15;
                anchors.verticalCenter: parent.verticalCenter;
                spacing: 10;
                Button {
                    icon: "hardhat_ico.svg";
                    color: "#00000000";
                    label: "";
                    height: detail_slide.height * 0.9;
                    width: detail_slide.height * 0.9;
                    onButtonClick: {
                        item_detail_slide.hide();
                        hour_detail_slide.hide();
                        user_detail_slide.toggle();
                    }
                }
                Button {
                    icon: "barcode_ico.svg";
                    color: "#00000000";
                    label: "";
                    height: detail_slide.height * 0.9;
                    width: detail_slide.height * 0.9;
                    onButtonClick: {
                        user_detail_slide.hide();
                        hour_detail_slide.hide();
                        item_detail_slide.toggle();
                    }
                }
                Button {
                    icon: "clock_ico.svg";
                    color: "#00000000";
                    label: "";
                    height: detail_slide.height * 0.9;
                    width: detail_slide.height * 0.9;
                    onButtonClick : {
                        user_detail_slide.hide();
                        item_detail_slide.hide();
                        hour_detail_slide.toggle();
                    }
                }

                Button {
                    icon: "print_ico.svg";
                    height: detail_slide.height * 0.9;
                    width: detail_slide.height * 0.9;
                    onButtonClick : {
                        App.choice("Print Report?");
                    }
                }
            }
        }
    }

    Rectangle {
        anchors.bottom: reportBox.top;
        anchors.horizontalCenter: reportBox.horizontalCenter;
        anchors.bottomMargin: 20;
        radius: 10;
        color: Colors.tran_white;
        height: 50;
        width: 250;
        Text {
            anchors.centerIn: parent;
            font.pointSize: 15;
            font.family: "Helvetica";
            text: selected;
            smooth: true;
            color: "black";
        }
    }

    Rectangle {
        id: quickview;
        anchors.fill: parent;
        color: "#dd000000";
        visible: false;

        onVisibleChanged : {
            if( visible ){
                ticket.y = height/2 - ticket.height/2;
            }else {
                ticket.y = -ticket.height;
            }
        }

        MouseArea {
            anchors.fill: parent;
            onClicked : {
                ticket.y = -ticket.height;
            }
        }

        Ticket{
            id: ticket;
            anchors.horizontalCenter: parent.horizontalCenter;
            y: -height;
            allowOk: true;
            editable: false;
            virtual: false;
            orderListModel: Reports.saleOrderModel;

            Behavior on y { NumberAnimation { duration: 150; easing.type: Easing.InOutCirc; } }

            onYChanged : {
                if( y == -height ) {
                    quickview.visible = false;
                }
            }

            onOkClick : {
                y = -height;
            }

            onPrintClick : {
                Reports.print_sale();
            }
        }
    }

    Loader {
        id: loader;
        anchors.fill: parent;
        source: "";
    }

    /*CashOutView {
        id: totals_report;
        dim: true;
        show_expected: false;
        color: Colors.make( Colors.violet3, "cc" );
        dimColor: "#99000000";
    }*/
}

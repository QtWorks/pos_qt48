//! NOTE:   QML supose to allow me to use vars
//          from including file. But this breaks
//          when using callbacks from inside js file
//          so these bottom variables are set everytime
//          menuview is created.
var ticketlib;
var app;
var menulib;
var manager_action;

function selectCategory(name, id) {
    //selected_category_name  = name;
    current_cat   = id;
    //var index = 0;
    var i = 0;

    for( i = 0; i < catModel.count; i++) {
        var item_id = catModel.get( i ).item_id;
        if( item_id === current_cat ) {
            return i;
            //index = i;
        }
    /*    else if( menulib.catItems[item_id].selected ) {
            menulib.catItems[item_id].deselect();
        }
        */
    }
    //return index;
}

function selectItem(name, id, issub) {
    selected_item_name  =   name;
    selected_item_id    =   id;
    if ( menulib.select( id ) ) {
        ticketlib.updateListAdd();
        ticket.ticketSubTotal = ticketlib.API.subTotal;
        ticket.ticketOwed = ticketlib.API.owed;
        ticket.seek_end();
        if( menulib.hassubs(id) ) {
            ticketlib.selectLastItem();
        }
    }
}

function editTicket() {

}

function showCategories() {
}

function navigateTo(item_id) {
    menulib.navigate(item_id);
}

function printAction( print ) {
    if( print ) {
        ticketlib.API.print( ticketlib.API.saleId );
        app.choice( "Another Copy?", printAction );
    }
}

function finalAction( save ) {
    if( save ) {
        ticketlib.finalize_changes();
        ticketlib.refresh();
        //app.hide_choice();
        app.setBackCB( null );
        app.choice( "Print Receipt?", printAction );
    }
    else {
        ticketlib.discard_changes();
        ticketlib.refresh();
    }

    app.navBack();
}

function backButtonPressed() {
    //Hijack the back press and apply save logic
   if( ticketlib.unsaved() || ticketlib.API.removed_count() ) {
       app.choice( "Would you like to commit changes?", finalAction );
   }
   else {
       app.setBackCB( null );
       app.navBack();
   }
}

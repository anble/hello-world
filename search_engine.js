var order_detail_id;

$(function() {
	var hostName = window.location.protocol + "//"+window.location.host+"/";

	$.extend( $.ui.autocomplete.prototype, {
	    _renderItem: function( ul, item ) {
	        var term = this.element.val(),
	        regex = new RegExp( '(' + term + ')', 'gi' );
            item.label = item.label;
              html = item.label.replace( regex , "<span style='color:blue;font-weight: bold;'>$&</span>" );
          ul.css( "zIndex", 20001);
	        return $( "<li></li>")
	            .data( "item.autocomplete", item )
	            .append( $("<a></a>").html(html) )
	            .appendTo( ul );
	    }
	});

    // used in webshop search field to get list of products
    $( "#searchinput_desktop,#searchinput_mobile" ).autocomplete({
      source: function( request, response ) {
        $.ajax( {
          url: hostName+"admin/modules/searchengine/searchengine.asp?f=2&v1="+encodeURIComponent(request.term)+"&v2=0",
          dataType: "json",
          success: function( data ) {
            response( data );
          }
        } );
      },
      minLength: 2
    } );


  } );

  $(function() {
    product_autocomplete();
    customer_autocomplete();
    product_report_autocomplete();
  });

var product_report_autocomplete = function(){
  var function_name = "product_report_autocomplete";
  var hostName = window.location.protocol + "//"+window.location.host+"/";
  // used in admin reports for getting product names
  $( "input[name='product_report']" ).autocomplete({
    source: function( request, response ) {
      $.ajax( {
        url: hostName+"admin/modules/searchengine/searchengine.asp?f=3&v1="+encodeURIComponent(request.term),
        dataType: "json",
        success: function( data ) {
        response( data );
        }
      } );
    },
      minLength: 2,
      select: function( event, ui ) {
      var this_label = new String(ui.item.label);
      var art_no = new String(this_label.match(/^\((.*?)\)/g));
      art_no = art_no.substring(1,art_no.length-1);
      $("input[name='data_product_id']").val(art_no);
      product_report_autocomplete();
      return false;
    }
  });
}

 var product_autocomplete = function(){
   var hostName = window.location.protocol + "//"+window.location.host+"/";
  
   // used in admin order detail for getting product names
    $( "input[name='product_name']" ).autocomplete({
      source: function( request, response ) {
         order_detail_id = new String($(this.element).prop("id"));
          order_detail_id = order_detail_id.substring(2,order_detail_id.length);
        $.ajax( {
          url: hostName+"admin/modules/searchengine/searchengine.asp?f=3&v1="+encodeURIComponent(request.term)+"&v2="+order_detail_id,
          dataType: "json",
          success: function( data ) {            
            response( data );           
           
          }
        } );
      },
      minLength: 2,
      select: function( event, ui ) {
        var this_label = new String(ui.item.label);
        var art_no = new String(this_label.match(/^\((.*?)\)/g));
        art_no = art_no.substring(1,art_no.length-1);
        var prod_name = new String(this_label.match(/\s.*/g));
        prod_name = prod_name.substring(1,prod_name.length);
        var ajaxUrl =hostName+"admin/modules/searchengine/searchengine.asp?f=4&v1="+art_no+"&v2="+order_detail_id;
        if($("#currency_checkbox").is(":checked"))
          ajaxUrl = ajaxUrl +"&currency_id="+ $("#order_currency").val();
        $.ajax( {
          url: ajaxUrl,
          dataType: "Json",
          success: function(data) {
            $("#or_"+order_detail_id).html($(data.row).html());
            $("#order_value").html(data.order_value);
            if(data.order_note){
              $("#order_note").val(data.order_note);
            }
            if(data.order_pending==1){
              $("input[name='order_pending']")[0].checked = true;
            }            
            if(data.row.indexOf("checked") > 0)
              $("#btn_drop_ship").removeClass("hide");
            if($("td.isgiftcard").length>0){
              $("#btGiftcard").removeClass("hidden");
            }
           product_autocomplete();

          }
        });
        return false;

      }
      
    });
   
  }

 var customer_autocomplete = function(){
   //-----------------------------------------------------------
   // used in admin order for getting customer name to create a order
   //-----------------------------------------------------------
   var hostName = window.location.protocol + "//"+window.location.host+"/";
    $("input[name='customer_name']").autocomplete({
      source: function( request, response ) {
        $.ajax( {
          url: hostName+"admin/modules/searchengine/searchengine.asp?f=5&v1="+encodeURIComponent(request.term),
          dataType: "json",
          success: function( data ) {
            response(data);
          }
        } );
      },
      minLength: 2,
      select: function( event, ui ) {
        var this_label = new String(ui.item.label);
        var cus_id = new String(this_label.match(/^\((.*?)\)/g));
        cus_id = cus_id.substring(1,cus_id.length-1);
        window.location.href= "admin.asp?sp_adminaction=3&sp_orderaction=13&sp_customerid="+ cus_id;
        return false;
        }
    });
  }

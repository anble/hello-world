
<script language="javascript" type="text/javascript" runat="server">

// *** Customer ***
//
//edit by dev_01 28/12/2017 === edit again 10:27AM
// dev 01 and dev 01 edit on the same file : dev 01 first
//====================
function show_customer_errors(isClear){
  var code="",
  errors = String(Session("errors"));

  if((errors != "") && (errors != "undefined")){
    code = code + '<scr'+'ipt type="text/javascript">';
    code = code + '$(document).ready(function() {';
    code = code + ' show_error_message("Error!","'+errors+'");';
    code = code + '});';
    code = code + '</scr'+'ipt>';
    if(isClear) Session("errors") = "";
  }
  return code;
}
//deit by dev_02
function view_customerdetail(is_ajaxrender)
{
  var function_name="view_customerdetail";
  var rs;
  var rs2;
  var code ="", info = "";
  var num_1="";
  var num_2="";
  var sql_query="";
  var err_desc="pos 0";
  var i18n = I18N.get_info();
  try {

    if (remove_null(Session("sp_customerid"))=="")
    {
      Session("sp_detailcustomeraction")=0;
      Session("sp_customeraction")=1;
      Session("sp_customeraddresscode")=0;
      Session("errors")="";
      return db_viewcustomer();
    }

    var conn = db_openconnection("order");
    switch (Session("sp_detailcustomeraction"))
    {
      case "6":
        approve_reseller_user();
        Session("sp_detailcustomeraction")=0;
        break;
    }

    code = show_customer_errors(true);
    rs = Server.CreateObject("ADODB.Recordset");
    rs2 = Server.CreateObject("ADODB.Recordset");
    sql_query="select * from alc_customer where customer_id="+Session("sp_customerid");
err_desc="pos 1: sql="+sql_query;
      rs.Open(sql_query,conn);

  code = code + '<div class="main-button">';

    thisurl=Session("base_url")+'admin.asp?sp_adminaction=1';
  code = code + small_button(WH.get_atext('common.buttons.main_menu'),thisurl,"");
    thisurl=Session("base_url")+'admin.asp?sp_adminaction=2';
  code = code + small_button(WH.get_atext('pages.customer.customer_menu'),thisurl,"");
    thisurl=Session("base_url")+'admin.asp?sp_adminaction=94';
  code = code + small_button(WH.get_atext('pages.customer.note'),thisurl,"");
    if (!rs.EOF)
    {
      thisurl=Session("base_url")+"admin.asp?sp_adminaction=3&amp;sp_orderaction=23&amp;sp_customerid="+new String(rs("customer_id"));
    code = code + small_button(WH.get_atext('pages.customer.new_order'),thisurl,"orange");
      thisurl=Session("base_url")+'admin.asp?sp_adminaction=3&amp;sp_orderaction=1&amp;sp_orderstatus=0&amp;sp_ordertaker=0&amp;sp_sqlorderid=';
      thisurl=thisurl+'&amp;sp_orderpage=1&amp;sp_invoiceno=&amp;&fast_date=yyyy-mm-dd&amp;q_paytype=&amp;q_trptype=&amp;';
      thisurl=thisurl+'fast_search=0&amp;sp_sqlname=&amp;customer_type='+new String(rs("customer_type"))+'&amp;sp_ordercustomer='+new String(rs("customer_id"));
    code = code + small_button(WH.get_atext('pages.customer.view_order'),thisurl,"blue");

    code = code + '</div>';

    code = code + '<div class="customer-detail">';

    code +='<div class="info">';
    info +='<h2>'+WH.get_atext("pages.customer.info")+'</h2>';
    info +='<table class="table-sm" id="tbl_customer_detail">';

    info +='<form name="frm_customertype" action="'+Session("base_url")+'admin.asp?sp_adminaction=73&sp_detailcustomeraction=3" method="post"></form>';

    //customer no
    info += '<tr class="text">';
    info +='<td class="text_right">';
    info +=WH.get_atext('pages.customer.customer_id');
    info +='</td>';
    var isRegistered = isRegisteredUser(rs("customer_id")), registerUserText = "", reseller_userid="";
    if(isRegistered){
      registerUserText = '<span class="system_message">'+WH.get_atext('pages.customer.register_user');
      if(this_customer_type == 2)
        reseller_userid=encode_string(remove_null(new String(rs("reseller_userid"))));

      sql_query = "SELECT DATE_FORMAT(timestamp,  '%m/%d/%Y %H:%i:%s') AS 'timestamp' FROM alc_customer_behaviour "
      + " WHERE (log_action = 'login' OR log_action = 'activate_account') AND status_action = '1' AND "
          + (reseller_userid != "" ? "customer_id = " + new String(rs("customer_id")) : " info LIKE '%"+remove_null(new String(rs("email_work")))+ "%' AND info LIKE '%\"is_reseller\":false%'")
      + " ORDER BY log_id DESC LIMIT 1;";
      var arr = wh_db2object(sql_query, db_openconnection("data"));
      if(arr && arr.length > 0)
      {
        registerUserText += (arr[0]["timestamp"] != "" ? " - Login " + arr[0]["timestamp"] : " - Unlogged in");
      }
      else
        registerUserText += " - Unlogged in";
      registerUserText += '</span>';
    }
    info +='<td class="text"><b>'+remove_null(new String(rs("customer_id")))+'</b>'+ registerUserText +'</td>';
    info +='</tr>';
    var this_customer_type=new String(rs("customer_type"));
        this_customer_type = Number(remove_null(this_customer_type));
    var is_company = (this_customer_type==Constants.CustomerTypes.COMPANY)?true:false;
    if (is_company){
      //company_name
      info +='<tr class="text">';
      info +='<td class="text_right">';
      info +=WH.get_atext('pages.customer.company_name');
      info +='</td>';
      company_name=encode_string(remove_null(new String(rs("company_name"))));
      info +='<td class="text"><input size="40" name="company_name" value="'+company_name+'"/></td>';
      info +='</tr>';
      //corporate_id
      info +='<tr class="text">';
      info +='<td class="text_right">';
      info +=WH.get_atext('pages.customer.corporate');
      info +='</td>';
      org_number=encode_string(remove_null(new String(rs("org_number"))));
      info +='<td class="text"><input size="20" name="org_number" value="'+org_number+'"/></td>';
      info +='</tr>';
    }
    //sales_responsible
    info +='<tr class="text">';
    info +='<td class="text_right">'+WH.get_atext('pages.customer.sales_responsible')+'</td>';
    info += '<td class="text" colspan="2">';
    var user_id = remove_null(new String(rs("sales_responsible_id")));
    info += '<select name="sales_responsible" onchange="update_sales_responsible();" tabindex="4">';
    info += '<option value = "0">{0}</option>'.format([WH.get_atext('common.texts.not_selected')]);
    info += get_dropdown_cb("alc_user","user_id","user_name","system",user_id);
    info += '</select>';
    info += '</td>';
    info +='</tr>';
    //reigster_status
    var confirmed = remove_null(String(rs("confirmed")));
    if(confirmed != null){
      info += '<tr class="text">';
      info += '<td class="text_right">';
      info += WH.get_atext('pages.customer.register_status');
      info += '</td>';
      info += '<td class="text">';
      info += '<select name="confirmed">';
      var arr_confirmed = eval("("+WH.get_atext("pages.customer.registration_status")+")");
      for (var key in arr_confirmed) {
          if (arr_confirmed.hasOwnProperty(key)) {
            info += '<option value="'+String(key)+'" '+(confirmed == String(key) ? "selected=\"selected\"": "")+'>'+arr_confirmed[key]+'</option>';
          }
      }
      info += '</select>';
      info += '</td>';
      info += '</tr>';
    }
    if (is_company){ // if company
      //user_name
      if(String(rs("company_type_id"))=="1"){
        info +='<tr class="text">';
        info +='<td class="text_right">';
        info +=WH.get_atext('pages.customer.user_name');
        info +='</td>';
        reseller_userid=encode_string(remove_null(new String(rs("reseller_userid"))));
        password_timestamp = remove_null(rs("password_timestamp"));
        info +='<td class="text"><input size="20" name="reseller_userid" value="'+reseller_userid+'"/>';
        thisurl="javascript:reseller_user_approval(73,6,"+Number(Session("sp_customerid"))+")";
        info +=widetiny_button(WH.get_atext('pages.customer.send_password'),thisurl,"orange");
        if (password_timestamp!="")
          info +='<span class="system_message" style="font-size:10px;width:100%;float:left">'+WH.get_atext('pages.customer.password_send')+' '+get_datetimestring(new Date(password_timestamp))+'</span>';
        info +='</td>';
        info +='</tr>';
      }
      //company_status
      info +='<tr class="text">';
      info +='<td class="text_right">';
      info +=WH.get_atext('pages.customer.company_status');
      info +='</td>';
      info +='<td class="text">';
      sql_query="select * from "+WH.get_tb("alc_company_status",i18n.current_lang_code)+" order by company_status_id";
      err_desc="pos 2: sql="+sql_query;
      rs2.Open(sql_query,conn);
        info += '<select class="text" name="company_status_id">';
      err_desc="pos 2.1: sql="+sql_query;
      num_1=remove_null(new String(rs("company_status_id")));
      if (num_1=="") num_1=5;
      err_desc="pos 2.2: sql="+sql_query+", company_status_id="+num_1;
      while (!rs2.eof)
      {
        num_2=new String(rs2("company_status_id"));
        info += '<option value="'+num_2+'" '+ (num_1+""==num_2+"" ? 'selected="selected"' : '')+'>'+new String(rs2("status_text"))+'</option>';
        rs2.MoveNext();
      }
      rs2.Close();
      info += '</select>';
      info += '</td>';
      info += '</tr>';
    }
    //customer_type
    info +='<tr class="text">';
    info +='<td class="text_right">';
    info +=WH.get_atext('pages.customer.customer_type');
    info +='</td>';
    info +='<td class="text">';

    sql_query="select * from "+WH.get_tb("alc_customer_type",i18n.current_lang_code)+" order by customer_type_id";
    err_desc="pos 2: sql="+sql_query;
    rs2.Open(sql_query,conn);
    info +='<select name="customer_type" onchange="update_customer(\'customer_type\');">';

      err_desc="pos 2.1: sql="+sql_query;
      var this_company_type =new String(rs("company_type_id"));
      err_desc="pos 2.2: sql="+sql_query+", customer_type="+this_customer_type;

      while (!rs2.eof)
      {
        num_2=new String(rs2("customer_type_id"));
            info += '<option value="'+num_2+'" '+ (this_customer_type+""==num_2+"" ? 'selected="selected"' : '')+'>'+new String(rs2("customer_type_name"))+'</option>';
        rs2.MoveNext();
      }
      rs2.Close();
        info +='</select>';
    info +='</td>';
    info +='</tr>';
    if (is_company){
      //company_type
      info +='<tr class="text">';
      info +='<td class="text_right">';
      info +=WH.get_atext('pages.customer.company_type');
      info +='</td>';
      info +='<td class="text">';
      sql_query="select * from "+WH.get_tb("alc_company_type",i18n.current_lang_code)+" order by company_type_id";
      err_desc="pos 2: sql="+sql_query;
      rs2.Open(sql_query,conn);
      num_1=remove_null(new String(rs("company_type_id")));
      if (num_1=="") num_1=4;
      info +='<select class="text" name="company_type" onchange="update_customer(\'company_type\');">';
      while (!rs2.eof)
      {
        num_2=new String(rs2("company_type_id"));
        info += '<option value="'+num_2+'" '+ (num_1+""==num_2+"" ? 'selected="selected"' : '')+'>'+new String(rs2("type_name"))+'</option>';
        rs2.MoveNext();
      }
      rs2.Close();
      info +='</select>';
      info +='</td>';
      info +='</tr>';
      //business_segment
      info +='<tr class="text">';
      info +='<td class="text_right">';
      info +=WH.get_atext('pages.customer.business_segment');
      info +='</td>';
      info += '<td class="text" colspan="2">';
      var business_segment_id = remove_null(new String(rs("business_segment_id")));
      info += '<select name="business_segment" onchange="update_business_segment_id();" tabindex="4">';
      info += '<option value = "0">{0}</option>'.format([WH.get_atext('common.texts.not_selected')]);
      info += get_dropdown_cb("alc_business_segment","business_segment_id","business_segment_name","order",business_segment_id);
      info += '</select>';
      info += '</td>';
      info += '</tr>';
      //type_of_business
      info +='<tr class="text">';
      info +='<td class="text_right">';
      info +=WH.get_atext('pages.customer.type_of_business');
      info +='</td>';
      info += '<td class="text" colspan="2">';
      var business_type_id = remove_null(new String(rs("business_type_id")));
      info += '<select name="business_type" onchange="update_customer_business_type();" tabindex="4">';
      info += '<option value = "0">{0}</option>'.format([WH.get_atext('common.texts.not_selected')]);
      info += get_dropdown_cb("alc_business_type","business_type_id","business_type_name","order",business_type_id);
      info += '</select>';
      info += '</td>';
      info +='</tr>';
    }
    info +='<form name="frm_updatecustomer" id="frm_updatecustomer" action="'+Session("base_url")+'admin.asp?sp_adminaction=73&sp_detailcustomeraction=2" method="post"></form>';
    info +='<input type="hidden" name="custtype" value="'+this_customer_type+'"/>';
    info +='<input type="hidden" name="companytype" value="'+this_company_type+'"/>';

    err_desc="pos 3: sql="+sql_query+", iscompany (2)="+num_1;
    if (is_company){ // if company
      info +='<tr class="text">';
      info +='<td class="text_right">';
      info +=WH.get_atext('pages.customer.company_reference');
      info +='</td>';
      company_reference=encode_string(remove_null(new String(rs("company_reference"))));
      info +='<td class="text"><input size="20" name="company_reference" value="'+company_reference+'"/></td>';
      info +='</tr>';

      info +='<tr class="text">';
      info +='<td class="text_right">';
      info +=WH.get_atext('pages.customer.website');
      info +='</td>';
      homepage_url=encode_string(remove_null(new String(rs("homepage_url"))));
      info +='<td class="text"><input size="40" name="homepage_url" value="'+homepage_url+'"/></td>';
      info +='</tr>';
      info +='<tr class="text">';
      info +='<td class="text_right">';
      info +=WH.get_atext('pages.customer.email_company');
      info +='</td>';
      email_home=encode_string(remove_null(new String(rs("email_home"))));
      info +='<td class="text"><input size="40" name="email_home" value="'+email_home+'"/></td>';
      info +='</tr>';

      //title
      if(get_setting_value("enable_title_customer") == "1"){
        info +='<tr class="text">';
        info +='<td class="text_right">';
        info +=WH.get_atext('pages.customer.title');
        info +='</td>';
        title=encode_string(remove_null(new String(rs("title"))));
        info +='<td class="text">'+dropdown_title("title", title)+'</td>';
        info +='</tr>';
      }

      info +='<tr class="text">';
      info +='<td class="text_right">';
      info +=WH.get_atext('pages.customer.contact_person_first_name');
      info +='</td>';
      first_name=encode_string(remove_null(new String(rs("first_name"))));
      info +='<td class="text"><input size="40" name="first_name" value="'+first_name+'"/></td>';
      info +='</tr>';
      info +='<tr class="text">';
      info +='<td class="text_right">';
      info +=WH.get_atext('pages.customer.last_name');
      info +='</td>';
      last_name=remove_null(encode_string(new String(rs("last_name"))));
      info +='<td class="text"><input size="40" name="last_name" value="'+last_name+'"/></td>';
      info +='</tr>';
      info +='<tr class="text">';
      info +='<td class="text_right">';
      info +=WH.get_atext('pages.customer.email_contact');
      info +='</td>';
      email_work=encode_string(remove_null(new String(rs("email_work"))));
        info +='<td class="text"><input size="40" name="email_work" value="'+email_work+'"/>';
        if(String(rs("company_type_id"))!="1"){
          info += ('   <div id="div_sendactivationlink" onclick="send_activate_link({2});" class="widesmall_button_orange" style="width:160px; float:none; cursor:pointer; {1}">{0}</div>').format([WH.get_atext('pages.customer.send_active_link'),(confirmed != "1" ? "display:none;" : ""),Session("sp_customerid")]);
          info += ('   <div id="div_resetpassword" onclick="reset_account_password({2});" class="widesmall_button_orange" style="width:160px; float:none; cursor:pointer; {1}">{0}</div>').format([WH.get_atext('pages.customer.reset_password'),(confirmed == "" || confirmed == "1" ? "display:none;" : ""),Session("sp_customerid")]);
        }

      info +='</td></tr>';
      info +='<tr class="text">';
      info +='<td class="text_right">';
      info +=WH.get_atext('pages.customer.voucher');
      info +='</td>';
      discount_level=encode_string(remove_null(new String(rs("discount_level"))));
      info +='<td class="text"><input size="5" name="discount_level" value="'+discount_level+'"/>&nbsp;'+WH.get_atext('pages.customer.value')+'</td>';
      info +='</tr>';
      info +='<tr class="text">';
      info +='<td class="text_right">';
      info +=WH.get_atext('pages.customer.term_of_payment');
      info +='</td>';
      payment_condition=encode_string(remove_null(new String(rs("payment_condition"))));
      info +='<td class="text"><input size="5" name="payment_condition" value="'+payment_condition+'"/>&nbsp;'+WH.get_atext('pages.customer.value')+'</td>';
      info +='</tr>';
    }
    else
    {
      err_desc="pos 4: sql="+sql_query+", iscompany (2)="+num_1;
      //title
      if(get_setting_value("enable_title_customer") == "1"){
        info +='<tr class="text">';
        info +='<td class="text_right">';
        info +=WH.get_atext('pages.customer.title');
        info +='</td>';
        title=encode_string(remove_null(new String(rs("title"))));
        info +='<td class="text">'+dropdown_title("title", title)+'</td>';
        info +='</tr>';
      }
      info +='<tr class="text">';
      info +='<td class="text_right">';
      info +=WH.get_atext('pages.customer.first_name');
      info +='</td>';
      first_name=encode_string(remove_null(new String(rs("first_name"))));
      info +='<td class="text"><input size="40" name="first_name" value="'+first_name+'"/></td>';
      info +='</tr>';
      info +='<tr class="text">';
      info +='<td class="text_right">';
      info +=WH.get_atext('pages.customer.last_name');
      info +='</td>';
      last_name=encode_string(remove_null(new String(rs("last_name"))));
      info +='<td class="text"><input size="40" name="last_name" value="'+last_name+'"/></td>';
      info +='</tr>';
      info +='<tr class="text">';
      info +='<td class="text_right">';
      info +=WH.get_atext('pages.customer.email');
      info +='</td>';
      info += '<td class="text">';
      email_work=encode_string(remove_null(new String(rs("email_work"))));
      info += '   <input size="40" name="email_work" value="'+email_work+'" style="float:left" />';
      info += '</td>';
      info +='</tr>';
      info += '<td>';
      info += '</td>';
      info += '<td>';

      //if(confirmed == "1"){
      info += ('   <div id="div_sendactivationlink" onclick="send_activate_link({2});" class="widesmall_button_orange" style="width:160px; float:none;cursor:pointer;{1}">{0}</div>').format([WH.get_atext('pages.customer.send_active_link'),(confirmed != "1" ? "display:none;" : ""),Session("sp_customerid")]);
      info += ('   <div id="div_resetpassword" onclick="reset_account_password({2});" class="widesmall_button_orange" style="width:160px; float:none;cursor:pointer;{1}">{0}</div>').format([WH.get_atext('pages.customer.reset_password'), (confirmed == "" || confirmed == "1" ? "display:none;" : ""),Session("sp_customerid")]);
      //}

      info += '</td>';
      // Check the address table to see the addresses of this customer
      sql_query = "SELECT * from alc_customer_address where customer_id = {0} ORDER BY is_default DESC".format([sqv(String(rs("customer_id")))]);
      var obj = wh_db2object(sql_query, conn);
      var hasAddresses=false;
      var address_1 = "",address_2 = "",postal_code = "",city = "", country_code = "";
      if(obj != null && obj.length > 0){

        hasAddresses=true;

        info +='<tr class="text">';
        info +='<td class="text_right">'+WH.get_atext('pages.customer.address')+'</td>';
        info +='<td class="text">';
        info +='<select name="customer_address_code" style="width:264px;">';

        for(var i in obj){
          var addressItem = "", selected="";
            if(obj[i].is_default == "true"){
                selected = "selected=\"selected\"";
                address_1 = obj[i].address_1;
                address_2 = obj[i].address_2;
                postal_code = obj[i].postal_code;
                city = obj[i].city;
                country_code = obj[i].country_code;
            }

            if (obj[i].address_1 == "" && obj[i].postal_code == "" && obj[i].city == ""  && obj[i].country_code == ""){
                addressItem += WH.get_atext('pages.customer.list_empty');
            }else {
                addressItem += "{0} {1}, {2}, {3} {4}".format([obj[i].address_1,
                          obj[i].address_2,
                          obj[i].postal_code,
                          obj[i].city,
                obj[i].country_code,
                obj[i].customer_address_id]);
            }
            info +='<option value="'+obj[i].customer_address_id+'">'+addressItem+'</option>';
        }

        info +='</select>';
        info +='</td>';
        info +='</tr>';
  // Address
        info +='<tr class="text">';
        info +='<td class="text_right">';
        info +=WH.get_atext('pages.customer.address');
        info +='</td>';
        info +='<td class="text"><input size="40" name="address_1" value="'+address_1+'"/></td>';
        info +='</tr>';
        info +='<tr class="text">';
        info +='<td class="text_right">';
        info +='</td>';

        info +='<td class="text"><input size="40" name="address_2" value="'+address_2+'"/></td>';
        info +='</tr>';
        info +='<tr class="text">';
        info +='<td class="text_right">';
        info +=WH.get_atext('pages.customer.zipcode');
        info +='</td>';

        info +='<td class="text"><input size="10" name="postal_code" value="'+postal_code+'"/></td>';
        info +='</tr>';
        info +='<tr class="text">';
        info +='<td class="text_right">';
        info +=WH.get_atext('pages.customer.town');
        info +='</td>';

        info +='<td class="text"><input size="40" name="city" value="'+city+'"/></td>';
        info +='</tr>';
        info +='<tr class="text">';
        info +='<td class="text_right">';
        info +=WH.get_atext('pages.customer.country');
        info +='</td>';
        info +='<td class="text">'+show_country("country_code", country_code, false, i18n.current_lang_code)+'</td>';
        info +='</tr>';
      }
    }


    if (is_company){ // if company
      info +='<tr class="text">';
      info +='<td class="text-group" colspan="2">';
      info +=WH.get_atext('pages.customer.delivery_address');
      info +='</td>';
      info +='</tr>';
    }
    if(!hasAddresses){
      info +='<tr class="text">';
      info +='<td class="text_right">';
      info +=WH.get_atext('pages.customer.address');
      info +='</td>';
    address_1=encode_string(remove_null(new String(rs("address_1"))));
      info +='<td class="text"><input size="40" name="address_1" value="'+address_1+'"/></td>';
      info +='</tr>';
      info +='<tr class="text">';
      info +='<td class="text_right">';
      info +='</td>';
    address_2=encode_string(remove_null(new String(rs("address_2"))));
      info +='<td class="text"><input size="40" name="address_2" value="'+address_2+'"/></td>';
      info +='</tr>';
      info +='<tr class="text">';
      info +='<td class="text_right">';
      info +=WH.get_atext('pages.customer.zipcode');
      info +='</td>';
    postal_code=encode_string(remove_null(new String(rs("postal_code"))));
      info +='<td class="text"><input size="10" name="postal_code" value="'+postal_code+'"/></td>';
      info +='</tr>';
      info +='<tr class="text">';
      info +='<td class="text_right">';
      info +=WH.get_atext('pages.customer.town');
      info +='</td>';
    city=encode_string(remove_null(new String(rs("city"))));
      info +='<td class="text"><input size="40" name="city" value="'+city+'"/></td>';
      info +='</tr>';
      info +='<tr class="text">';
      info +='<td class="text_right">';
      info +=WH.get_atext('pages.customer.country');
      info +='</td>';
        country_code=encode_string(remove_null(new String(rs("country_code"))));
            info +='<td class="text">'+show_country("country_code", country_code, false, i18n.current_lang_code)+'</td>';
      info +='</tr>';
      }

    err_desc="pos 5: sql="+sql_query+", iscompany (2)="+num_1;
    if (is_company){ // if company
      var same_address = encode_string(remove_null(new String(rs("same_address"))));
      info +='<tr class="text">';
      info +='<td class="text-group" colspan="2">';
      info +=WH.get_atext('pages.customer.invoice_address');
      info +='</td>';
      info +='</tr>';
      info +='<tr class="text">';
      info +='<td class="text_right">';
      info +=WH.get_atext('common.texts.name');
      info +='</td>';
      var invoice_name=encode_string(remove_null(new String(rs("invoice_name"))));
      info +='<td class="text"><input size="40" name="invoice_name" value="'+invoice_name+'"/></td>';
      info +='</tr>';
            info +='<tr class="text" name="trDelivery"' + (same_address == '1' ? 'style="display:none"' : '') + '>';
      info +='<td class="text_right">';
      info +=WH.get_atext('pages.customer.address');
      info +='</td>';
      var invoice_address=encode_string(remove_null(new String(rs("invoice_address"))));
      info +='<td class="text"><input size="40" name="invoice_address" value="'+invoice_address+'"/></td>';
      info +='</tr>';
            info +='<tr class="text" name="trDelivery"' + (same_address == '1' ? 'style="display:none"' : '') + '>';
      info +='<td class="text_right">';
      info +='</td>';
      var invoice_address2=encode_string(remove_null(new String(rs("invoice_address2"))));
      info +='<td class="text"><input size="40" name="invoice_address2" value="'+invoice_address2+'"/></td>';
      info +='</tr>';
            info +='<tr class="text" name="trDelivery"' + (same_address == '1' ? 'style="display:none"' : '') + '>';
      info +='<td class="text_right">';
      info +=WH.get_atext('pages.customer.zipcode');
      info +='</td>';
      var invoice_postalcode=encode_string(remove_null(new String(rs("invoice_postalcode"))));
      info +='<td class="text"><input size="10" name="invoice_postalcode" value="'+invoice_postalcode+'"/></td>';
      info +='</tr>';
            info +='<tr class="text" name="trDelivery"' + (same_address == '1' ? 'style="display:none"' : '') + '>';
      info +='<td class="text_right">';
      info +=WH.get_atext('pages.customer.town');
      info +='</td>';
      var invoice_city=encode_string(remove_null(new String(rs("invoice_city"))));
      info +='<td class="text"><input size="40" name="invoice_city" value="'+invoice_city+'"/></td>';
      info +='</tr>';
      info +='<tr class="text">';
      info +='<td class="text-group" colspan="2">';
      info +='</td>';
      info +='</tr>';
    }

    info +='<tr class="text">';
    info +='<td class="text_right">';
    info +=WH.get_atext('pages.customer.mobile');
    info +='</td>';
    phone_fixed=encode_string(remove_null(new String(rs("phone_fixed"))));
    info +='<td class="text"><input size="20" name="phone_fixed" value="'+phone_fixed+'"/></td>';
    info +='</tr>';
    info +='<tr class="text">';
    info +='<td class="text_right">';
    info +=WH.get_atext('pages.customer.phone');
    info +='</td>';
    phone_mobile=encode_string(remove_null(new String(rs("phone_mobile"))));
    info +='<td class="text"><input size="20" name="phone_mobile" value="'+phone_mobile+'"/></td>';
    info +='</tr>';
    info +='<tr class="text">';
    info +='<td class="text_right">';
    info +=WH.get_atext('pages.customer.fax');
    info +='</td>';
    phone_fax=encode_string(remove_null(new String(rs("phone_fax"))));
    info +='<td class="text"><input size="20" name="phone_fax" value="'+phone_fax+'"/></td>';
    info +='</tr>';

    info +='<tr class="text">';
    info +='<td class="text" colspan="2">';
    info += '<b>'+WH.get_atext('pages.customer.comment')+'</b>';
    customer_comment=encode_string(remove_null(new String(rs("customer_comment"))));
    info +='<br><textarea rows="5" style="width:100%" name="customer_comment">'+customer_comment+'</textarea></td>';
    info +='</tr>';

err_desc="pos 6: sql="+sql_query+", iscompany (2)="+num_1;
    info +='</td>';
    info +='</tr>';
    info +='</form>';
    info +='</table>';

    if(is_ajaxrender != true)
        code += info;

    code +='</div>'; // info div
  // user comments
    code = code + '<div class="customer-orders-notes">';
      // Contact person
    code += '<div id="section_contact_persons">'
    if (this_customer_type==2) { // if company
      code += show_contact_persons()
    }
    code += '</div>';

    code = code + show_notes(rs("customer_id"),0);

    code = code + '<div class="notes_stats">';
    code = code + '<h2>'+WH.get_atext('pages.customer.order')+'</h2>';
    code = code + show_orderrows4customer(rs("customer_id"));
    code = code + '</div>';
    code = code + '</div>';
  }
  else
  {
    code = code + '<br><br>'+WH.get_atext('pages.customer.found_no_customer');
    code = code + '</div>';
  }

  rs.Close();
  db_closeconnection(conn);

  }
  catch (e)
  {
    print_error(e,function_name,err_desc);
  }

  if(is_ajaxrender == true)
      return info;
    else
      return code;
}

  function auto_insert_contact_person(first_name, last_name, email_work, phone_fixed)
{
    var function_name="auto_insert_contact_person";
    var rs, conn;
    var sql="", result=0, err_desc = "pos 0";

    if(remove_null(email_work) == "" &&  remove_null(phone_fixed) == "" &&  remove_null(first_name) == ""){
        return;
    }
    //save_trace2file(function_name, first_name + "|" +  last_name + "|" +  email_home + "|" +  phone_fixed);
    try {
        conn= db_openconnection("order");

        rs = Server.CreateObject("ADODB.Recordset");
        sql="SELECT COUNT(contact_id) FROM alc_customer_contacts WHERE is_default = 1 AND customer_id="+Session("sp_customerid");
        err_desc = "pos 1, sql=" + sql;
        rs_open(rs,sql,conn);
        if(!rs.EOF){
            if(Number(rs(0)) <= 0){
                err_desc = "pos 2, first_name=" + first_name + ", last_name=" + last_name + ", email_home=" + email_work + ", phone_fixed=" + phone_fixed + ", sp_customerid=" + Session("sp_customerid");
                //var obj = {"first_name": encode_string_todb(remove_null(first_name)),
                //    "last_name": encode_string_todb(remove_null(last_name)),
                //    "email_work": encode_string_todb(remove_null(email_home)),
                //    "phone_fixed": encode_string_todb(remove_null(phone_fixed)),
                //    "is_default": true,
                //    "customer_id": Session("sp_customerid")
                //};
                //sql = wh_getdbquery(obj,"alc_customer_contacts","insert");
                sql = 'INSERT INTO alc_customer_contacts (first_name, last_name, email_work, phone_fixed, is_default, customer_id) '
                + 'VALUES ("'+encode_string_todb(remove_null(first_name))
                +'","'+encode_string_todb(remove_null(last_name))
                +'","'+encode_string_todb(remove_null(email_work))
                +'","'+ encode_string_todb(remove_null(phone_fixed))
                +'",1,'+Session("sp_customerid")+')';

                err_desc = "pos 3, sql=" + sql;
                conn.execute(sql);
            }
        }
    }
    catch (e)
    {
        print_error(e,function_name,err_desc);
    }
    finally
    {
        rs_close(rs);
        db_closeconnection(conn);
    }

    return result;
}

function save_contact_person()
{
    var function_name="save_contact_person";

    var rs, conn;
    var sql="", result=0, err_desc = "pos 0";

    try {
        conn= db_openconnection("order");

        rs = Server.CreateObject("ADODB.Recordset");
        //var obj = {}

        var is_default = (String(remove_null(Request.Form("is_default"))) == "on" ? true : false);
        if(is_default){
            sql = "UPDATE alc_customer_contacts SET is_default = 0 WHERE customer_id = " + Session("sp_customerid");
            err_desc = "pos 1, sql=" + sql;
            conn.execute(sql);
        }

        var contact_id = Number(remove_null(Request.Form("contact_id")));
        if(contact_id != "")
        {// update
            //obj = {"first_name": encode_string_todb(remove_null(Request.Form("first_name"))),
            //    "last_name": encode_string_todb(remove_null(Request.Form("last_name"))),
            //    "email_work": encode_string_todb(remove_null(Request.Form("email_work"))),
            //    "phone_fixed": encode_string_todb(remove_null(Request.Form("phone_fixed"))),
            //    "phone_other": encode_string_todb(remove_null(Request.Form("phone_other"))),
            //    "comment": encode_string_todb(remove_null(Request.Form("comment"))),
            //    "customer_role_id": (remove_null(Request.Form("customer_role_id")) != "" ? remove_null(Request.Form("customer_role_id")) : null),
            //    "customer_id": Session("sp_customerid"),
            //    "is_default":is_default,
            //    "where": "contact_id=" + contact_id
            //};

            sql = 'UPDATE alc_customer_contacts SET '
            + 'first_name="' +  encode_string_todb(remove_null(Request.Form("first_name")))
            + '", last_name="' + encode_string_todb(remove_null(Request.Form("last_name")))
            + '", email_work="' + encode_string_todb(remove_null(Request.Form("email_work")))
            + '", phone_fixed="' + encode_string_todb(remove_null(Request.Form("phone_fixed")))
            + '", phone_other="' + encode_string_todb(remove_null(Request.Form("phone_other")))
            + '", comment="' + encode_string_todb(remove_null(Request.Form("comment")))
            + '", customer_role_id=' + (remove_null(Request.Form("customer_role_id")) != "" ? remove_null(Request.Form("customer_role_id")) : null)
            + ', is_default=' +  (remove_null(Request.Form("is_default")) != "" ? 1 : 0)
            + ' WHERE contact_id = ' + contact_id;
            err_desc = "pos 2, sql=" + sql;
            //sql = wh_getdbquery(obj,"alc_customer_contacts","update");
        }
        else
        {// add new
            //obj = {"first_name": encode_string_todb(remove_null(Request.Form("first_name"))),
            //    "last_name": encode_string_todb(remove_null(Request.Form("last_name"))),
            //    "email_work": encode_string_todb(remove_null(Request.Form("email_work"))),
            //    "phone_fixed": encode_string_todb(remove_null(Request.Form("phone_fixed"))),
            //    "phone_other": encode_string_todb(remove_null(Request.Form("phone_other"))),
            //    "comment": encode_string_todb(remove_null(Request.Form("comment"))),
            //    "customer_role_id":  (remove_null(Request.Form("customer_role_id")) != "" ? remove_null(Request.Form("customer_role_id")) : null),
            //    "is_default": is_default,
            //    "customer_id": Session("sp_customerid")
            //};

            sql = 'INSERT INTO alc_customer_contacts (first_name,last_name,email_work,phone_fixed,phone_other,comment,customer_role_id,is_default,customer_id) VALUES '
            + '("' +  encode_string_todb(remove_null(Request.Form("first_name")))
            + '", "' + encode_string_todb(remove_null(Request.Form("last_name")))
            + '", "' + encode_string_todb(remove_null(Request.Form("email_work")))
            + '", "' + encode_string_todb(remove_null(Request.Form("phone_fixed")))
            + '", "' + encode_string_todb(remove_null(Request.Form("phone_other")))
            + '", "' + encode_string_todb(remove_null(Request.Form("comment")))
            + '", ' + (remove_null(Request.Form("customer_role_id")) != "" ? remove_null(Request.Form("customer_role_id")) : null)
            + ', ' +  (remove_null(Request.Form("is_default")) != "" ? 1 : 0)
            + ', ' + Session("sp_customerid") + ')';
            err_desc = "pos 2, sql=" + sql;
            //sql = wh_getdbquery(obj,"alc_customer_contacts","insert");
        }

        conn.execute(sql);

        if(contact_id == ""){
            var obj = wh_db2object("SELECT contact_id FROM alc_customer_contacts WHERE customer_id = " +Session("sp_customerid") + " ORDER BY contact_id DESC LIMIT 1;", conn);
            result = obj[0].contact_id;
        }
        else{
            result = contact_id;
        }
    }
    catch (e)
    {
        print_error(e,function_name,sql);
    }
    finally
    {
        db_closeconnection(conn);
    }

    return result;
}

function show_contact_persons()
{
    var function_name="show_contact_persons";

    var rs;
    var code ="";
    var sql="";
    var err_desc="pos0";
    var conn;


  try
  {
    conn= db_openconnection("order");
    rs = Server.CreateObject("ADODB.Recordset");
    code += '<div class="notes_stats">';
    code += '<h2>'+WH.get_atext('pages.customer.contact_person')+'</h2>';

    //Button action
    code + '<div class="notes_stats">';
    code += widetiny_button(WH.get_atext('common.buttons.new'),'javascript:add_contact_persons();',"orange");
    code += '</div>';
    //Popup
    //Dropdown Role
    sql = "SELECT * FROM alc_customer_role";
    rs_open(rs, sql, conn);
    var dd = "";
    while(!rs.EOF)
    {
        dd += '<option value="'+ rs("role_id")+'">'+ rs("role_name")+'</option>';
        rs.MoveNext;
    }

    var defContact = WH.get_atext('pages.customer.default_contact');
        code += '<div id="contact_persons">'+
                 '<form method="post" id="frm_contactperson" name="frm_contactperson">'+
                '<fieldset class="contact-person">'+
                '<legend id="h_contact_person"></legend>'+
                    '<input type="hidden" name="contact_id" />'+
                    '<table class="table-sm">'+
                        '<tr class="text">'+
                            '<td class="text right" style="width:80px;">Role</td>'+
                            '<td class="text">'+
                            '<select id="customer_role_id" name="customer_role_id" class="form-control">' +
                            '<option value=""></option>' + dd +
                            '</select>' +
                            '<label class="ct_label"><input type="checkbox" id="is_default" name="is_default" /><span>'+defContact+'</span></label>' +
                            '</td>'+
                        '</tr>'+
                        '<tr class="text">'+
                            '<td class="text right">'+WH.get_atext('pages.customer.first_name')+'</td>'+
                            '<td class="text"><input class="form-control" type="text" size="40" name="first_name" value=""></td>'+
                        '</tr>'+
                        '<tr class="text">'+
                            '<td class="text right">'+WH.get_atext('pages.customer.last_name')+'</td>'+
                            '<td class="text"><input class="form-control" type="text" size="40" name="last_name" value=""></td>'+
                        '</tr>'+
                        '<tr class="text">'+
                            '<td class="text right">'+WH.get_atext('pages.customer.phone_work')+'</td>'+
                            '<td class="text"><input class="form-control" type="text" size="40" name="phone_fixed" value=""></td>'+
                        '</tr>'+
                        '<tr class="text">'+
                            '<td class="text right">'+WH.get_atext('pages.customer.phone_other')+'</td>'+
                            '<td class="text"><input class="form-control" type="text" size="40" name="phone_other" value=""></td>'+
                        '</tr>'+
                        '<tr class="text">'+
                            '<td class="text right">'+WH.get_atext('pages.customer.email_work')+'</td>'+
                            '<td class="text"><input class="form-control" type="text" size="40" name="email_work" value=""></td>'+
                        '</tr>'+
                        '<tr class="text">'+
                            '<td class="text right">'+WH.get_atext('pages.customer.comment')+'</td>'+
                            '<td class="text"><textarea rows="6" cols="43" name="comment"></textarea></td>'+
                        '</tr>'+
                        '<tr class="text">'+
                            '<td class="text right" colspan="2">'+small_button('Save',"javascript:save_contact_persons();","orange")+small_button('Cancel',"javascript:close_contact_persons();","grey")+'</td>'+
                        '</tr>'+
                    '</table>'+
                '</fieldset>'+
                '</form>'+
                '</div>';

        //Contact person list
        code += '<div class="notes_stats">' +
                    '<table id="tbl_contact_person" class="table table-bordered-input">' +
        //Header
        '<tr class="text">' +
            '<th class="text" width="3%"></th>' +
            '<th class="text"width="3%"></th>' +
            '<th class="text"width="3%"></th>' +
            '<th class="text">Role</th>' +
            '<th class="text" width="20%">'+WH.get_atext('pages.customer.first_name')+'</th>' +
            '<th class="text" width="20%">'+WH.get_atext('pages.customer.last_name')+'</th>' +
            '<th class="text" width="30%">'+WH.get_atext('pages.customer.email_work')+'</th>' +
            '<th class="text" style="max-width:120px;">'+WH.get_atext('pages.customer.phone_work')+'</th>' +
        '</tr>';

        sql = "SELECT cc.*, cr.role_name FROM alc_customer_contacts cc LEFT JOIN alc_customer_role cr ON cr.role_id = cc.customer_role_id WHERE customer_id = " + Session("sp_customerid");
        rs_open(rs, sql, conn)
        while(!rs.EOF)
        {
            var first_name = remove_null(String(rs("first_name"))),
                last_name = remove_null(String(rs("last_name"))),
                email_work = remove_null(String(rs("email_work"))),
                phone_fixed = remove_null(String(rs("phone_fixed"))),
                comment = remove_null(String(rs("comment")));

            code += '<tr class="text" id="'+rs("contact_id")+'">'+
                    '<td><a href="javascript:add_contact_persons('+rs("contact_id")+')"><i class="fa fa-pencil-square-o" aria-hidden="true"></i></a></td>'+
                    '<td><a href="javascript:delete_contact_persons('+rs("contact_id")+')"><i class="fa fa-trash-o" aria-hidden="true"></i></a></td>'+
                    '<td>'+(Number(rs("is_default")) > 0 ? '<i class="checkbox fa fa-check-square-o" aria-hidden="true"></i>' : '<i class="fa fa-square-o" aria-hidden="true"></i>' )+'</td>'+
                    '<td>'+remove_null(String(rs("role_name")))+'</td>'+
                    '<td name="td_first_name">'+first_name+'</td>'+
                    '<td name="td_last_name">'+last_name+'</td>'+
                    '<td name="td_email_work">'+email_work+'</td>'+
                    '<td name="td_phone_fixed">'+phone_fixed+'</td>'+
                    //'<td>'+comment+'</td>'+
                '</tr>';
            rs.MoveNext;
        }

    code = code + '</table>';
    code += '</div>';

    rs.close();
    db_closeconnection(conn);
  }
  catch (e){
    print_error(e,function_name,err_desc);
  }

  return code;
}

function db_viewcustomer() {
  var function_name="db_viewcustomer";
  var rs;
  var code ="";
  var sql_query="";
  var err_desc="pos0";
  var i18n = I18N.get_info(), req=request_info();
    var base_url = get_base_url()+"/admin/";
  try {
    var sp_customeraction = get_session("sp_customeraction");
    switch (String(sp_customeraction)){
      case "2":
        add_customer();
        remove_session("sp_customeraction");
        Session("sp_adminaction")=73;
        code=view_customerdetail();
      return code;

      case "3":
        update_customer_overview();
        remove_session("sp_customeraction");
      break;

      case "4":
        result=delete_customer();
        if (result>0)
          code=code+system_message(WH.get_atext('pages.customer.message_delete_customer_id1')+''+Session("sp_customerid")+''+WH.get_atext('pages.customer.message_delete_customer_id2')+''+result+''+WH.get_atext('pages.customer.message_delete_customer_id3')+'<br />');
        remove_session("sp_customeraction");
      break;

      case "5":
        erase_qfields();
        remove_session("sp_customeraction");
      break;
    }

    var conn;
    conn= db_openconnection("order");
    code = code + '<div class="main-button">';
    thisurl=base_url+'admin.asp?sp_adminaction=1';
    code = code + small_button(WH.get_atext("common.buttons.main_menu"),thisurl,"");
    code = code + '</div>';

    code = code+insert_searchcustomer(req);
    count_where=0;
    where_clause="";
    search_text=WH.get_atext('pages.customer.search_parameters');
    var q_customer_id = get_session("q_customer_id");
    if(q_customer_id != ""){
      where_clause += (count_where > 0)?" and customer_id='{0}'".format([q_customer_id])
                                       :" where customer_id='{0}'".format([q_customer_id]);
      count_where++;
      search_text += WH.get_atext('pages.customer.customer_id_is')+q_customer_id;
    }

    var q_customer_company = get_session("q_customer_company");
    if (q_customer_company!=""){
      where_clause += (count_where>0)?" and company_name like '%{0}%'".format([q_customer_company])
                                     :" where company_name like '%{0}%'".format([q_customer_company]);
      count_where++;
      search_text += WH.get_atext('pages.customer.company_is')+q_customer_company;
    }

    var q_customer_lastname = get_session("q_customer_lastname");
    if (q_customer_lastname != ""){
      where_clause +=(count_where>0)?" and last_name like '%{0}%'".format([q_customer_lastname])
                                    :" where last_name like '%{0}%'".format([q_customer_lastname]);
      count_where++;
      search_text +=WH.get_atext('pages.customer.lastname_is')+q_customer_lastname;
    }

    var q_customer_firstname = get_session("q_customer_firstname");
    if (q_customer_firstname != ""){
      where_clause += (count_where>0)?" and first_name like '%{0}%'".format([q_customer_firstname])
                                     :" where first_name like '%{0}%'".format([q_customer_firstname]);
      count_where++;
      search_text += WH.get_atext('pages.customer.first_name_is')+q_customer_firstname;
    }

    var q_customer_city = get_session("q_customer_city");
    if (q_customer_city != ""){
      where_clause += (count_where>0)?" and city like '%{0}%'".format([q_customer_city])
                                     :" where city like '%{0}%'".format([q_customer_city]);
      count_where++;
      search_text += WH.get_atext('pages.customer.town_is')+q_customer_city;
    }

    var q_customer_email=get_session("q_customer_email");
    if (q_customer_email!=""){
      where_clause += (count_where>0)?" and email_work like '%{0}%'".format([q_customer_email])
                                       :" where email_work like '%{0}%'".format([q_customer_email]);
      count_where++;
      search_text=search_text+WH.get_atext('pages.customer.mail_is')+q_customer_email;
    }

    var q_customer_type = get_session("q_customer_type");
    if(q_customer_type!="%" && q_customer_type!="") {
      where_clause += (count_where>0)?" and customer_type='{0}'".format([q_customer_type])
                                     :" where customer_type='{0}'".format([q_customer_type]);
      count_where++;
      search_text +=WH.get_atext('pages.customer.customer_type_is')+q_customer_type;
    }

    var q_company_type = get_session("q_company_type");
    if (q_company_type != "%" && q_company_type != ""){
      where_clause +=(count_where > 0)?" and company_type_id='{0}'".format([q_company_type])
                                      :" where company_type_id='{0}'".format([q_company_type]);
      count_where++;
      search_text +=WH.get_atext('pages.customer.company_type_is')+q_company_type;
    }

    var q_company_status = get_session("q_company_status");
    if (q_company_status!="%" && q_company_status!=""){
      where_clause += (count_where>0)?" and company_status_id='{0}'".format([q_company_status])
                                     :" where company_status_id='{0}'".format([q_company_status]);
      count_where++;
      search_text +=WH.get_atext('pages.customer.company_status_is')+q_company_status;
    }

    var sales_responsible = get_session("sales_responsible");
    if (sales_responsible!="%" && sales_responsible!=""){
      where_clause += (count_where>0)?" and sales_responsible_id='{0}'".format([sales_responsible])
                                     :" where sales_responsible_id='{0}'".format([sales_responsible]);
      count_where++;
      search_text +=WH.get_atext('pages.customer.sales_responsible_id_is')+type_of_business;
    }

    var type_of_business = get_session("type_of_business");
    if (type_of_business!="%" && type_of_business!=""){
      where_clause += (count_where>0)?" and business_type_id='{0}'".format([type_of_business])
                                     :" where business_type_id='{0}'".format([type_of_business]);
      count_where++;
      search_text +=WH.get_atext('pages.customer.type_of_business_is')+type_of_business;
    }

    var business_segment = get_session("business_segment");
    if (business_segment!="%" && business_segment!=""){
      where_clause += (count_where>0)?" and business_segment_id='{0}'".format([business_segment])
                                     :" where business_segment_id='{0}'".format([business_segment]);
      count_where++;
      search_text +=WH.get_atext('pages.customer.business_segment_is')+business_segment;
    }

  rs = Server.CreateObject("ADODB.Recordset");
  sql_query="select count(customer_id) as count from alc_customer" + where_clause;
  var conts = wh_db2object2(sql_query,"order");
  count_records=Number(conts[0].count);

  sql_query="select company_type_id, type_name from "+WH.get_tb("alc_company_type",i18n.current_lang_code)+" order by company_type_id";
  var comp_types = wh_db2object2(sql_query, "order");
  var this_type_array = new Array();
  for(var i=0;comp_types && comp_types.length && i<comp_types.length;i++){
    this_type_array[comp_types[i].company_type_id]=comp_types[i].type_name;
  }

  sql_query="select company_status_id, status_text from "+WH.get_tb("alc_company_status",i18n.current_lang_code)+" order by company_status_id;";
  err_desc="pos4 sql="+sql_query;
  var comp_statuses = wh_db2object2(sql_query,"order");
  var this_status_array = new Array();
  for(var i=0;comp_statuses && comp_statuses.length && i<comp_statuses.length;i++){
    this_status_array[comp_statuses[i].company_status_id]=comp_statuses[i].status_text;
  }

  var sql_query = "select country_code,country,for_top_list from {0} order by for_top_list desc, country asc".format([WH.get_tb("alc_country_code", i18n.current_lang_code)]);
  var countries  = wh_db2object2(sql_query,"order");
  var html_country ="";
  if(countries && countries.length){
    html_country = "<select class='form-control' name='{0}' style='width:150px'>".format(["country_code"]);
    html_country += "<option value=''>Empty</option>"
    for(var i=0;i<countries.length;i++) {
      html_country += "<option value='{0}'>{1}</option>".format([String(countries[i].country_code),String(countries[i].country)]);
    }
    html_country += '</select>';
  }

  max_pageitems=50;
  start_page=Number(get_session("sp_customerpage"));
  start_point = (start_page-1)*max_pageitems;
  end_page=(count_records<=max_pageitems)?1:Math.ceil(count_records/max_pageitems);
  title=WH.get_atext('pages.customer.customer_list');
  if (search_text) title=title+": "+search_text;

  code = code + '<table class="table-sm">';
  code = code + '<tr class="text"><td class="text">';

  var actions = [];
  thisurl='javascript:document.forms.frm_customer.submit();';
  actions.push({label:WH.get_atext('common.buttons.update'),url:thisurl});
  thisurl=base_url+'admin.asp?sp_customeraction=2&amp;sp_customerpage='+start_page;
  actions.push({label:WH.get_atext('common.buttons.new'),url:thisurl});
  code += action_button_panel(actions);
  code += paging_control(Session("base_url")+"admin.asp","sp_customerpage",start_page,end_page,"("+count_records+"&nbsp;"+WH.get_atext('common.texts.result')+")");
  code = code + '</td></tr>';
  code += '<tr class="text"><td class="text"><span class="search-text-param">'+title+'</span></td></tr></table>';
  var orderby=get_session("q_customer_order"), order_direction=get_session("q_customer_direction");
  if(orderby && order_direction)
    where_clause=where_clause+" order by {0} {1}".format([orderby,order_direction]);
  sql_query="select * from alc_customer"+where_clause;
  if(!where_clause) sql_query += " ORDER BY customer_id DESC";
  rs.Open(sql_query,conn);

  code = code + '<form method="post" name="frm_customer" action="'+Session("base_url")+'admin.asp?sp_customeraction=3">';
  code += '<div class="search-results">';
  code = code + '<table class="table-sm table-bordered-input table-hover">';
  code = code + '<tr class="text">';
  code = code + '<th class="text">';
  code = code + WH.get_atext('pages.customer.order');
  code = code + '</th>';
  code = code + '<th class="text">';
  code = code + WH.get_atext('common.texts.customer');
  code = code + '</th>';
  code = code + '<th class="text">';
  code = code + WH.get_atext('pages.customer.p_f');
  code = code + '</th>';
  code = code + '<th class="text">';
  code = code + WH.get_atext('pages.customer.company_type');
  code = code + '</th>';
  code = code + '<th class="text">';
  code = code + WH.get_atext('pages.customer.status');
  code = code + '</th>';
  code = code + '<th class="text">';
  code = code + WH.get_atext('pages.customer.company1');
  code = code + '</th>';
  code = code + '<th class="text" colspan="2">';
  code = code + WH.get_atext('common.texts.name');
  code = code + '</th>';
  code = code + '<th class="text">';
  code = code + WH.get_atext('pages.customer.address');
  code = code + '</th>';
  code = code + '<th class="text">';
  code = code + WH.get_atext('pages.customer.zip_code');
  code = code + '</th>';
  code = code + '<th class="text">';
  code = code + WH.get_atext('pages.customer.city1');
  code = code + '</th>';
  code = code + '<th class="text">';
  code = code + WH.get_atext('pages.customer.country');
  code = code + '</th>';
  code = code + '<th class="text">';
  code = code + WH.get_atext('pages.customer.email');
  code = code + '</th>';
  code = code + '<th class="text">';
  code = code + WH.get_atext('pages.customer.mobile');
  code = code + '</th>';
  code = code + '<th class="text">&nbsp;';
  code = code + '</th>';
  code = code + '</tr>';

  myeven=false;

  if (!rs.EOF)
  {
    while (!rs.EOF && start_point>0)
    {
      rs.MoveNext();
      start_point--;
    }

  }
  count_items=0;

  while (!rs.EOF && count_items<max_pageitems)
  {
    err_desc="pos6 count_items="+count_items;

    if (myeven)
      code = code + '<tr>';
    else
      code = code + '<tr>';
    code = code + '<td class="text">';
    code = code + '<div class="tiny-button-panel">';
    var items = [];
    thisurl=Session("base_url")+"admin.asp?sp_adminaction=3&amp;sp_orderaction=23&amp;sp_customerid="+new String(rs("customer_id"));
    //code = code + tiny_button(,thisurl,"orange");
    items.push({url: thisurl, label: WH.get_atext('common.buttons.new')});
    code += tiny_button(WH.get_atext('common.buttons.new'),thisurl,"blue");
    thisurl=Session("base_url")+'admin.asp?sp_adminaction=3&amp;fast_search=0&amp;sp_orderstatus=0&amp;sp_ordertaker=0&amp;customer_type='+new String(rs("customer_type"));
    thisurl=thisurl+'&amp;sp_sqlorderid=&amp;sp_sqlname=&amp;sp_orderpage=1&amp;sp_invoiceno=&amp;sp_orderaction=1&amp;sp_ordercustomer='+new String(rs("customer_id"));
    //code = code + tiny_button(,thisurl,"blue");
    items.push({url: thisurl, label: WH.get_atext('pages.customer.view')});
    //code = code + dropdown_menu(items);
    code += tiny_button(WH.get_atext('pages.customer.view'),thisurl,"blue");
    code = code + '</div>';
    code = code + '</td>';

    code = code + '<td class="text center">';
    code = code + '<input name="customer_id" type="hidden" value="'+new String(rs("customer_id"))+'" />';
    thisurl=Session("base_url")+'admin.asp?sp_adminaction=73&amp;sp_customerid='+new String(rs("customer_id"));
//    code = code + tiny_button(new String(rs("customer_id")),thisurl,"blue");
    code = code + '<a href="'+thisurl+'">'+new String(rs("customer_id"))+'</a>';
    code = code + '</td>';
    code = code + '<td class="text center">';
    if (rs("customer_type")==1)
      code = code + '<b>'+WH.get_atext('pages.order.customer_p')+'</b>';
    else
      code = code + '<b>'+WH.get_atext('pages.order.customer_f')+'</b>';
    code = code + '</td>';
    code = code + '<td class="text left">';
   // if (rs("customer_type")==2)
		// 	code = code + this_type_array[rs("company_type_id")];
		// code = code + '</td>';
		// code = code + '<td class="text left ">';
		// if (rs("customer_type")==2)
		// 	code = code + this_status_array[rs("company_status_id")];
		// replaced by an.b.le , fix a bug#547		
		if (rs("customer_type")==2){
			if(isNull(rs("company_type_id")))  
				code = code +WH.get_atext('common.texts.not_selected') ;
			else
				code = code + this_type_array[rs("company_type_id")];
			}			
			else{
				if(isNull(rs("company_type_id")))
					code = code +WH.get_atext('common.texts.not_selected');
				else
					code = code + this_type_array[rs("company_type_id")];
			
			}
		
		code = code + '</td>';
		code = code + '<td class="text left ">';		
		if (rs("customer_type")==2)
			if(isNull(rs("company_status_id")))  
					code = code +WH.get_atext('common.texts.not_selected');
			else
					code = code + this_type_array[rs("company_status_id")];		
    	//end bug#547	
    code = code + '</td>';
    code = code + '<td class="text">';
    code = code + '<input class="form-control" size="20" name="company_name" value="'+encode_string(remove_null(new String(rs("company_name"))))+'" />';
    code = code + '</td>';
    code = code + '<td class="text">';
    code = code + '<input class="form-control" size="13" name="first_name" value="'+encode_string(remove_null(new String(rs("first_name"))))+'" />';
    code = code + '</td>';
    code = code + '<td class="text">';
    code = code + '<input class="form-control" size="18" name="last_name" value="'+encode_string(remove_null(new String(rs("last_name"))))+'" />';
    code = code + '</td>';
    code = code + '<td class="text">';
    code = code + '<input class="form-control" size="20" name="address1" value="'+encode_string(remove_null(new String(rs("address_1"))))+'" />';
    code = code + '</td>';
    code = code + '<td class="text">';
    code = code + '<input class="form-control" size="6" name="postal_code" value="'+remove_null(new String(rs("postal_code")))+'" />';
    code = code + '</td>';
    code = code + '<td class="text">';
    code = code + '<input class="form-control" size="15" name="city" value="'+encode_string(remove_null(new String(rs("city"))))+'" />';
    code = code + '</td>';
    code = code + '<td class="text">';
    if(remove_null(String(rs("country_code"))).toUpperCase() == ""){
      code = code +  html_country;
    }
    else{
      var html_country_use = html_country;
      html_country_use = html_country_use.replace("'"+String(rs("country_code")).toUpperCase()+"'", "'"+String(rs("country_code")).toUpperCase()+"' selected=selected");

      //html_country_use = html_country_use.replace("<select", "<select el='"+ String(rs("country_code")) + "'");
      code = code +  html_country_use;//'<input class="form-control" size="10" name="country_code" value="'+encode_string(remove_null(new String(rs("country"))))+'" />';
    }

    code = code + '</td>';
    code = code + '<td class="text">';
    code = code + '<input class="form-control" size="30" name="email_work" value="'+remove_null(String(rs("email_work")))+'" disabled=disabled />';
    code = code + '</td>';
    code = code + '<td class="text">';
    code = code + '<input class="form-control" size="15" name="phone_fixed" value="'+remove_null(new String(rs("phone_fixed")))+'" />';
    code = code + '</td>';
    thismsg=WH.get_atext('pages.customer.confirm_delete_id1')+"&quot;"+new String(rs("customer_id"))+"&quot;";
    code = code + '<td class="text"><a href="javascript:if (window.confirm(\''+thismsg+'\')) location.href=\''+Session("base_url")+'admin.asp?sp_customeraction=4&amp;sp_customerid='+new String(rs("customer_id"))+'\'"><img src="./images/trashcan.gif" alt="'+WH.get_atext('common.texts.delete')+'"></img></a>';
    code = code + '</td>';
    code = code + '</tr>';

    rs.MoveNext();
    myeven=!myeven;
    count_items++;
  }

  code = code + '</table>';
  code += '</div>';
  code = code + '</form>';

  code = code + '</td>';
  code = code + '</tr>';
  code = code + '</table>';
  rs.Close();
  db_closeconnection(conn);
  }
  catch (e)
  {
    print_error(e,function_name,err_desc);
  }
  return code;
}

function db_viewcustomerlog()
{
    var function_name="db_viewcustomerlog";
    var rs;
    var code ="", sql="", err_desc="pos0";
    var i18n = I18N.get_info();

    try {
        switch (Session("sp_customeraction"))
        {
            case "1":
                code=view_customerdetail();
                //save_trace2file(function_name, "customer_id=" + Request.QueryString("customer_id"));
                Session("sp_customeraction") = 0;
                //Session("sp_adminaction")=179;
                Response.Write("##" + show_customer_log(remove_null(Request.QueryString("customer_id"))) + "##");
                return;
                break;
        }

        Session("sp_customeraction") = 0;
        var conn = db_openconnection("order");
        var conn_data = db_openconnection("data");

        thisurl=Session("base_url")+'admin.asp?sp_adminaction=1';
        code += '<scr'+'ipt>';
        code += '$( function() {';
        code += '    $("#txtFromDate").datepicker();$("#txtFromDate").datepicker("option", "dateFormat", "yy-mm-dd");';
        code += '    $("#txtToDate").datepicker();$("#txtToDate").datepicker("option", "dateFormat", "yy-mm-dd");';
        code += '});';
        code += '</scr'+'ipt>';
        code += '<table>';
        code += '   <tr>';
        code += '       <td class="text">';
        code += small_button(WH.get_atext('common.buttons.main_menu'),thisurl,"");
        code += '       </td>';
        code += '   </tr>';
        code += '</table>';

        code = code+insert_search_log_action();
        where_clause = " WHERE 1=1 ";
    search_text=WH.get_atext('pages.customer.search_criteria');
        if (!isNaN(session("q_customer_id")) && Session("q_customer_id")!=""){
            where_clause += " AND customer_id="+session("q_customer_id");
      search_text=search_text + WH.get_atext('pages.customer.customer_id_is')+"="+Session("q_customer_id");
        }

        if (remove_null(session("q_customer_company"))!=""){
            where_clause += " AND company_name LIKE '%"+session("q_customer_company")+"%'";
            search_text += WH.get_atext('pages.customer.company_is')+"="+Session("q_customer_company");
        }

        if (remove_null(session("q_customer_lastname"))!=""){
            where_clause += " AND last_name LIKE '%"+session("q_customer_lastname")+"%'";
            search_text += +Session("q_customer_lastname");
        }

        if (remove_null(session("q_customer_firstname"))!=""){
            where_clause += " AND first_name LIKE '%"+session("q_customer_firstname")+"%'";
      search_text += WH.get_atext('pages.customer.first_name_is')+Session("q_customer_firstname");
        }

        if (remove_null(session("q_customer_city"))!=""){
            where_clause+= " AND city LIKE '%"+session("q_customer_city")+"%'";
            search_text += WH.get_atext('pages.customer.town_is')+Session("q_customer_city");
        }

        if (remove_null(session("q_customer_email"))!=""){
            where_clause += " AND email_work LIKE '%"+session("q_customer_email")+"%'";
            search_text = search_text+WH.get_atext('pages.customer.mail_is')+Session("q_customer_email");
        }

        if (session("q_customer_type")!="%" && remove_null(session("q_customer_type"))!=""){
            where_clause += " AND customer_type="+session("q_customer_type");
            search_text += WH.get_atext('pages.customer.customer_type_is')+Session("q_customer_type");
        }

        if (session("q_company_type")!="%" && remove_null(session("q_company_type"))!=""){
            where_clause += " AND company_type_id="+session("q_company_type");
            search_text += WH.get_atext('pages.customer.company_type_is')+Session("q_company_type");
        }

        if (session("q_company_status")!="%" && remove_null(session("q_company_status"))!=""){
            where_clause += " AND company_status_id="+session("q_company_status");
            search_text += WH.get_atext('pages.customer.company_status_is')+Session("q_company_status");
        }

        var customer_list_log = "", where_log = "";
        if (remove_null(session("txtFromDate"))!=""){
            where_log += " AND timestamp >= '"+Session("txtFromDate") + "'";
            search_text += WH.get_atext('pages.customer.from_date')+Session("txtFromDate");
        }
        if (remove_null(session("txtToDate"))!=""){
            where_log += " AND timestamp <= '"+Session("txtToDate") + " 23:59:59'";
            search_text += WH.get_atext('pages.customer.to_date')+Session("txtToDate");
        }
        if (session("drpActionType")!="%" && remove_null(session("drpActionType"))!=""){
            var arr = session("drpActionType").split(",");
            where_log += " AND log_action IN (";
            for(var i=0;i<arr.length;i++)
            {
                if(i > 0)
                    where_log += ",";
                where_log += "'"+arr[i]+"'"
            }
            where_log += ')';
            search_text += WH.get_atext('pages.customer.action_type1')+Session("drpActionType");
        }

        if (remove_null(session("txtIP"))!=""){
            where_log += " AND ip = '"+String(Session("txtIP")).trim()+"'";
            search_text += WH.get_atext('pages.customer.ip_is')+Session("txtIP");
        }

        if (session("drpActionStatus")!="%" && remove_null(session("drpActionStatus"))!=""){
            where_log += " AND status_action = " + session("drpActionStatus");
            search_text += ", status_action="+Session("drpActionStatus");
        }

        rs = Server.CreateObject("ADODB.Recordset");
        sql="SELECT DISTINCT customer_id FROM alc_customer_behaviour WHERE customer_id > 0 " + where_log + " ORDER BY log_id DESC";

        rs.open(sql, conn_data);

        if (!rs.eof)
        {
          for(;!rs.eof; rs.MoveNext()){
            customer_list_log+=(customer_list_log==""?"":",")+String(rs("customer_id"));
          }
          where_clause += " AND customer_id IN ("+customer_list_log+")";
        }
        rs.close();
        sql='SELECT COUNT(customer_id) as count FROM alc_customer'+where_clause;
        err_desc="pos2 sql="+sql;
        rs_open(rs, sql, conn);
        if (!rs.eof)
            count_records=new Number(rs("count"));
        else
            count_records=0;

        sql="SELECT * FROM "+WH.get_tb("alc_company_type",i18n.current_lang_code)+" ORDER BY company_type_id";
        err_desc="pos3 sql="+sql;
        rs_open(rs, sql, conn);
        var this_type_array = new Array;
        while (!rs.eof)
        {
            this_type_array[rs("company_type_id")]=new String(rs("type_name"));
            rs.movenext();
        }

        sql="SELECT * FROM "+WH.get_tb("alc_company_status",i18n.current_lang_code)+" ORDER BY company_status_id";
        err_desc="pos4 sql="+sql;
        rs_open(rs, sql, conn);
        var this_status_array = new Array;
        while (!rs.eof)
        {
            this_status_array[rs("company_status_id")]=new String(rs("status_text"));
            rs.movenext();
        }

        max_pageitems=50;
        start_page=new Number(Session("sp_customerpage"));
        start_point = (start_page-1)*max_pageitems;
        if (count_records<=max_pageitems)
            end_page=1;
        else
            end_page = Math.ceil(count_records/max_pageitems);

    title=WH.get_atext('pages.customer.customer_list');
        if (search_text!="")
            title=title+" - "+search_text;
        code += '<table class="standard"><tr class="standard"><td class="standard">&nbsp;&nbsp;'+title+'</td></tr>';
        code += '<tr class="portlet_body"><td class="portlet_body">';

        code += '<table class="text">';
        code += '<tr><td class="text"></td>';
        code += '<td class="text"">';

        if (start_page>1)
        {
            code += '<form name="frm_prev" method="get" action="'+Session("base_url")+'admin.asp">';
            code += '<input type="hidden" name="sp_customerpage" value ="'+eval(start_page-1)+'"/>'
            code += '<a class="text" href="javascript:document.frm_prev.submit();"><<&nbsp;'+WH.get_atext('common.texts.prev_page')+'</a>';
            code += '</form>';
        }
        code += '</td><td class="text">';
        code += '<form name="frm_page" method="get" action="'+Session("base_url")+'admin.asp">';
        code += insert_dropdown(1,end_page,start_page,"javascript:document.frm_page.submit();","sp_customerpage");
        code += '</form>';
        code += '</td><td class="text">';
        /**/
        if (start_page<end_page)
        {
            code += '<form name="frm_next" method="get" action="'+Session("base_url")+'admin.asp">';
            code += '<input type="hidden" name="sp_customerpage" value ="'+eval(start_page+1)+'"/>'
            code += '<a class="text_right" href="javascript:document.frm_next.submit();">'+WH.get_atext('common.texts.next_page')+'&nbsp;>></a>';
            code += '</form>';
        }
        code += '</td><td class="text">';
        code += '</td>';
        code += '<td class="text">'+WH.get_atext('common.texts.page')+'&nbsp;'+Session("sp_customerpage")+'&nbsp;'+WH.get_atext('common.texts.of')+'&nbsp;'+end_page+'&nbsp;';
        code += '&nbsp;&nbsp;|&nbsp;&nbsp;'+count_records+'&nbsp;'+WH.get_atext('common.texts.result')+'&nbsp;';
        code += '</td>';
        code += '</tr>';

        code += '</table>';

        where_clause += (customer_list_log != "" ? " ORDER BY FIELD(customer_id, "+customer_list_log+")" : " ORDER BY customer_id DESC");

        sql="SELECT * FROM alc_customer"+where_clause;
        //save_trace2file(function_name, sql);
        err_desc="pos5 sql="+sql;
        rs_open(rs, sql,conn);

        code += '<form method="post" id="frm_customer" name="frm_customer" action="'+Session("base_url")+'admin.asp??sp_adminaction=179&sp_customeraction=1">';
        code += '<table class="text" id="customer_log_action">';
        code += '<tr class="text" id="header">';
        //code += '<td class="text_center">';
        //code += 'Order';
        //code += '</td>';
        code += '<td class="text_center">';
        code += WH.get_atext('common.texts.customer');
        code += '</td>';
        code += '<td class="text">';
        code += WH.get_atext('pages.customer.p_f');
        code += '</td>';
        code += '<td class="text">';
        code += WH.get_atext('pages.order.type');
        code += '</td>';
        code += '<td class="text">';
        code += WH.get_atext('pages.customer.status');
        code += '</td>';
        code += '<td class="text">';
        code += WH.get_atext('pages.customer.company1');
        code += '</td>';
        code += '<td class="text" colspan="2">';
        code += WH.get_atext('common.texts.name');
        code += '</td>';
        code += '<td class="text">';
        code += WH.get_atext('pages.customer.address');
        code += '</td>';
        code += '<td class="text">';
        code += WH.get_atext('pages.customer.zip_code');
        code += '</td>';
        code += '<td class="text">';
        code += WH.get_atext('pages.customer.town');
        code += '</td>';
        code += '<td class="text">';
        code += WH.get_atext('pages.customer.country');
        code += '</td>';
        code += '<td class="text">';
        code += WH.get_atext('green.email');
        code += '</td>';
        code += '<td class="text">';
        code += WH.get_atext('pages.customer.mobile');
        code += '</td>';
        code += '</tr>';

        myeven=false;

        if (!rs.EOF)
        {
            while (!rs.EOF && start_point>0)
            {
                rs.MoveNext();
                start_point--;
            }

        }
        count_items=0;

        while (!rs.EOF && count_items<max_pageitems)
        {
            err_desc="pos6 count_items="+count_items;
            thisurl = 'javascript:show_log_customer_detail('+new String(rs("customer_id"))+');';
            code += '<tr onclick="'+thisurl+'" class="admin';
            if (!myeven)
                code += '_bg';
            code += '">';

            //code += '<td class="text_center">';
            //thisurl=Session("base_url")+"admin.asp?sp_adminaction=3&amp;sp_orderaction=23&amp;sp_customerid="+new String(rs("customer_id"));
            //code += tiny_button("Ny",thisurl,"orange");
            //thisurl=Session("base_url")+'admin.asp?sp_adminaction=3&amp;fast_search=0&amp;sp_orderstatus=0&amp;sp_ordertaker=0&amp;customer_type='+new String(rs("customer_type"));
            //thisurl=thisurl+'&amp;sp_sqlorderid=&amp;sp_sqlname=&amp;sp_orderpage=1&amp;sp_invoiceno=&amp;sp_orderaction=1&amp;sp_ordercustomer='+new String(rs("customer_id"));
            //code += tiny_button("Visa",thisurl,"blue");
            //code += '</td>';
            code += '<td class="text_center">';
            code += '<input name="customer_id" type="hidden" value="'+new String(rs("customer_id"))+'" />';
            //thisurl=Session("base_url")+'admin.asp?sp_adminaction=73&amp;sp_customerid='+new String(rs("customer_id"));

            //    code += tiny_button(new String(rs("customer_id")),thisurl,"blue");
            code += new String(rs("customer_id"));
            code += '</td>';
            code += '<td class="text_center">';
            if (rs("customer_type")==1)
                code += '<b>'+WH.get_atext('pages.order.customer_p')+'</b>';
            else
                code += '<b>'+WH.get_atext('pages.order.customer_f')+'</b>';
            code += '</td>';
            code += '<td class="text_center">';
            if (rs("customer_type")==2)
                code += this_type_array[rs("company_type_id")];
            code += '</td>';
            code += '<td class="text_center">';
            if (rs("customer_type")==2)
                code += this_status_array[rs("company_status_id")];
            code += '</td>';
            code += '<td class="text">';
            code += encode_string(remove_null(new String(rs("company_name"))));
            code += '</td>';
            code += '<td class="text">';
            code += encode_string(remove_null(new String(rs("first_name"))));
            code += '</td>';
            code += '<td class="text">';
            code += encode_string(remove_null(new String(rs("last_name"))));
            code += '</td>';
            code += '<td class="text">';
            code += encode_string(remove_null(new String(rs("address_1"))));
            code += '</td>';
            code += '<td class="text">';
            code += remove_null(new String(rs("postal_code")));
            code += '</td>';
            code += '<td class="text">';
            code += encode_string(remove_null(new String(rs("city"))));
            code += '</td>';
            code += '<td class="text">';
            code += encode_string(remove_null(new String(rs("country"))));
            code += '</td>';
            code += '<td class="text">';
            code += remove_null(new String(rs("email_work")));
            code += '</td>';
            code += '<td class="text">';
            code += remove_null(new String(rs("phone_fixed")));
            code += '</td>';
            code += '</tr>';
            code += '<tr><td colspan="13" id="td-'+new String(rs("customer_id"))+'" style="display:none"></td></tr>';
            rs.MoveNext();
            myeven=!myeven;
            count_items++;
        }

        code += '</table>';
        code += '</form>';

        code += '</td>';
        code += '</tr>';
        code = code + '</table>';


    }
    catch (e)
    {
        print_error(e,function_name,err_desc);
    }
    finally
    {
        rs_close(rs);
        db_closeconnection(conn);
    }
    return code;
}

function show_customer_log(customer_id)
{
    var function_name="show_customer_log";
    var rs;
    var code ="Something wrong! customer_id doesn't have value.", sql="", err_desc="pos0";
    //save_trace2file(function_name, customer_id);
    if(customer_id == '' || customer_id == null)
        return code;

    try
    {
        var conn = db_openconnection("data");
        rs = Server.CreateObject("ADODB.Recordset");
        var where_log = "";
        if (remove_null(session("txtFromDate"))!=""){
            where_log += " AND timestamp >= '"+Session("txtFromDate") + "'";
        }
        if (remove_null(session("txtToDate"))!=""){
            where_log += " AND timestamp <= '"+Session("txtToDate") + " 23:59:59'";
        }
        if (session("drpActionType")!="%" && remove_null(session("drpActionType"))!=""){
            var arr = session("drpActionType").split(",");
            where_log += " AND log_action IN (";
            for(var i=0;i<arr.length;i++)
            {
                if(i > 0)
                    where_log += ",";
                where_log += "'"+arr[i]+"'"
            }
            where_log += ')';
        }
        sql='SELECT DATE_FORMAT(timestamp,  "%m/%d/%Y %H:%i:%s") AS "timestamp", user_action, page_id, info, log_action, log_id, status_action, from_url, ip FROM alc_customer_behaviour WHERE customer_id = "' + customer_id + '" ' + where_log + ' ORDER BY log_id DESC LIMIT 10;';
        err_desc="pos2 sql="+sql;
        //save_trace2file(function_name, sql);
        rs_open(rs, sql, conn);
        code = '<table class="customer-log" cellspacing="1" cellpadding="1">';
        if(!rs.eof)
        {
            code += '    <tr>';
            code += '        <th>'+WH.get_atext('pages.customer.date')+'</th>';
            code += '        <th>'+WH.get_atext('pages.customer.action')+'</th>';
            code += '        <th>'+WH.get_atext('pages.customer.status')+'</th>';
            code += '        <th>Url</th>';
            code += '        <th>IP</th>';
            code += '        <th style="max-width:400px">'+WH.get_atext('pages.customer.infomation')+'</th>';
            code += '    <tr>';
        }
        else
        {
            code += '    <tr>';
            code += '        <td>'+WH.get_atext('pages.customer.not_file_log_for_customer')+'!</td>';
            code += '    <tr>';
        }

        while(!rs.eof)
        {
            code += '<tr>';
            code += '   <td>' + rs("timestamp") + '</td>';

            //page
            var page = '';
            switch(String(rs("page_id")))
            {
                case "5":
                    page = "product detail";
                    break;
                case "10":
                    page = "404";
                    break;
                case "14":
                    page = "my pages";
                    break;
                case "37":
                    page = "cart payment";
                    break;
                case "4":
                    page = "checkout payment";
                    break;
                case "6":
                    page = "reseller";
                    break;
                case "8":
                    page = "category";
                    break;
            }

            code += '   <td>' + String(rs("log_action")) + '<br />>> '+ String(rs("user_action")) +' </td>';
            //status_action
            var status_action = '';
            switch(String(rs("status_action")))
            {
                case "0":
                    status_action = "undefinded";
                    break;
                case "1":
                    status_action = "success";
                    break;
                case "2":
                    status_action = "failed";
                    break;
            }
            code += '   <td>' + status_action + '</td>';
            code += '   <td>' + String(rs("from_url")) + (page != "" ? '<br />>> '+page : '') + '</td>';
            code += '   <td>' + String(rs("ip")) + '</td>';
            var log = remove_null(String(rs("info")));
            code += '   <td style="max-width:400px">';
            if(log != "")
                code += '<pre id="log_' + String(rs("log_id")) + '">' + log + '</pre>';

            code += '</td>';
            code += '</tr>';
            rs.MoveNext();
        }

        code += '</table>';
    }
    catch (e)
    {
        print_error(e,function_name,err_desc);
    }
    finally
    {
        rs_close(rs);
        db_closeconnection(conn);
    }
    return code;
}

function insert_search_log_action()
{
    var function_name="insert_search_log_action";

    var code="";

    code += '<form name="frm_search" class="product_search" action="'+Session("base_url")+'admin.asp" method="get">';
    code += '<input type="hidden" name="sp_customerpage" value ="1"/>'

  title=WH.get_atext('pages.customer.search_customer');
    code += '<table class="standard"><tr class="standard"><td class="standard">&nbsp;&nbsp;'+title+'</td></tr>';
    code += '<tr class="portlet_body"><td class="portlet_body">';

    code += '<table class="product_search">';

    code += '<tr class="product_search">';
    code += '<td class="text">'+WH.get_atext('pages.customer.customer_id')+'</td>';
    code += '<td class="text"><input class="text" type="text" name="q_customer_id" size="6" value="" tabindex="1"></td>';
    code += '<td></td>';
    code += '<td class="text">'+WH.get_atext('pages.customer.first_name')+'</td>';
    code += '<td class="text"><input class="text" type="text" name="q_customer_firstname" size="30" value="" tabindex="3"></td>';
    code += '<td class="text">'+WH.get_atext('pages.customer.company1')+'</td>';
    code += '<td class="text"><input class="text" type="text" name="q_customer_company" size="30" value="" tabindex="3"></td>';
    code += '</tr>';

    code += '<tr class="product_search">';
    code += '<td class="text">'+WH.get_atext('common.texts.sort_order')+'</td>';
    code += '<td class="text">';
    code += '<select class="text" name="q_customer_order" tabindex="4">';
    code += '<option value="customer_id" selected="selected">'+WH.get_atext('pages.customer.customer_id')+'</option>';
    code += '<option value="last_name">'+WH.get_atext('pages.customer.last_name')+'</option>';
    code += '<option value="first_name">'+WH.get_atext('pages.customer.first_name')+'</option>';
    code += '<option value="city">'+WH.get_atext('pages.customer.town')+'</option>';
    code += '</select>';
    code += '</td>';
    code += '<td>';
    code += '<select class="text" name="q_customer_direction" tabindex="4">';
    code += '<option value="DESC" selected="selected">'+WH.get_atext('pages.customer.desc')+'</option>';
    code += '<option value="ASC">'+WH.get_atext('pages.customer.asc')+'</option>';
    code += '</select>';
    code += '</td>';
    code += '<td class="text">'+WH.get_atext('pages.customer.last_name')+'</td>';
    code += '<td class="text"><input class="text" type="text" name="q_customer_lastname" size="30" value="" tabindex="3"></td>';
    code += '<td class="text">'+WH.get_atext('pages.customer.town')+'</td>';
    code += '<td class="text"><input class="text" type="text" name="q_customer_city" size="30" value="" tabindex="3"></td>';
    code += '</tr>';

    code += '<tr class="product_search">';
    code += '<td class="text">'+WH.get_atext('pages.customer.customer_type')+'</td>';
    code += '<td class="text" colspan="2">';
    code += '<select class="text" name="q_customer_type" tabindex="4">';
    code += '<option value="%" selected="selected">'+WH.get_atext('common.texts.all')+'</option>';
    code += '<option value="1">'+WH.get_atext('pages.customer.private')+'</option>';
    code += '<option value="2">'+WH.get_atext('pages.customer.company1')+'</option>';
    code += '</select>';
    code += '</td>';
    code += '<td class="text">'+WH.get_atext('pages.customer.email')+'</td>';
    code += '<td class="text"><input class="text" type="text" name="q_customer_email" size="30" value="" tabindex="3"></td>';
    code += '<td colspan="2">';
    code += '</td>';
    code += '</tr>';

    code += '<tr class="product_search">';
    code += '<td class="text">'+WH.get_atext('pages.customer.company_type')+'</td>';
    code += '<td class="text" colspan="2">';
    code += '<select class="text" name="q_company_type" tabindex="4">';
    code += '<option value="%" selected="selected">'+WH.get_atext('common.texts.all')+'</option>';
    code += '<option value="1">'+WH.get_atext('common.texts.reseller')+'</option>';
    code += '<option value="2">'+WH.get_atext('common.texts.end_customer')+'</option>';
    code += '<option value="3">'+WH.get_atext('common.texts.tsa')+'</option>';
    code += '<option value="4">'+WH.get_atext('common.texts.not_selected')+'</option>';
    code += '</select>';
    code += '</td>';
    code += '<td class="text">'+WH.get_atext('common.texts.company_type')+'</td>';
    code += '<td class="text" colspan="3">';
    code += '<select class="text" name="q_company_status" tabindex="4">';
    code += '<option value="%" selected="selected">'+WH.get_atext('common.texts.all')+'</option>';
    code += '<option value="1">'+WH.get_atext('common.texts.prospectus')+'</option>';
    code += '<option value="2">'+WH.get_atext('common.texts.in_active')+'</option>';
    code += '<option value="3">'+WH.get_atext('common.texts.customer')+'</option>';
    code += '<option value="4">'+WH.get_atext('common.texts.suspect')+'</option>';
    code += '<option value="5">'+WH.get_atext('common.texts.not_selected')+'</option>';
    code += '</select>';
    code += '</td>';
    code += '</tr>';

    //Line separate
    code += '<tr><td colspan="7"><hr style="border-top: 1px dashed Silver;" /></td></tr>';

    //log_action
    code += '<tr class="product_search">';
    code += '<td class="text">'+WH.get_atext('pages.product.from_day')+'</td>';
    code += '<td class="text" colspan="2">';
    code += '<input class="text" type="text" name="txtFromDate" id="txtFromDate" placeholder="yyyy-mm-dd" tabindex="5" />';
    code += '</td>';
    code += '<td class="text">'+WH.get_atext('pages.product.to_day')+'</td>';
    code += '<td class="text">';
    code += '<input class="text" type="text" name="txtToDate" id="txtToDate" placeholder="yyyy-mm-dd" tabindex="5" />';
    code += '</td>';
    code += '<td class="text">'+WH.get_atext('pages.customer.action_type')+'</td>';
    code += '<td class="text">';
    code += '<select class="text" name="drpActionType" tabindex="5">';
    code += '<option value="%" selected="selected">'+WH.get_atext('common.texts.all')+'</option>';
    code += '<option value="login">'+WH.get_atext('common.texts.login')+'</option>';

    //Registered
    code += '<option value="register,activate_account,logout,forgot_password">'+WH.get_atext('pages.customer.registered')+'</option>';
    code += '<option value="register">-- '+WH.get_atext('pages.customer.register')+'</option>';
    code += '<option value="activate_account">-- '+WH.get_atext('pages.customer.activate_account')+'</option>';
    code += '<option value="forgot_password">-- '+WH.get_atext('pages.customer.forgot_password')+'</option>';
    code += '<option value="logout">-- '+WH.get_atext('pages.customer.log_out')+'</option>';

    //My pages
    code += '<option value="show_order_details,update_password,add_customer_address,update_customer_address,delete_customer_address">'+WH.get_atext('pages.customer.action_on_my_page')+'</option>';
    code += '<option value="update_password">-- '+WH.get_atext('pages.customer.update_password')+'</option>';
    code += '<option value="add_customer_address">-- '+WH.get_atext('pages.customer.add_customer_address')+'</option>';
    code += '<option value="update_customer_address">-- '+WH.get_atext('pages.customer.update_customer_address')+'</option>';
    code += '<option value="delete_customer_address">-- '+WH.get_atext('pages.customer.delete_customer_address')+'</option>';
    code += '<option value="show_order_details">-- '+WH.get_atext('pages.customer.show_order_detail')+'</option>';

    //Revise cart
    code += '<option value="add_basket,changed_qty,cart_delete,check_out_top_page,check_out_popup_cart">'+WH.get_atext('pages.customer.revise_basket')+'</option>';
    code += '<option value="add_basket">-- '+WH.get_atext('pages.customer.add_to_basket')+'</option>';
    code += '<option value="changed_qty">-- '+WH.get_atext('pages.customer.change_qty')+'</option>';
    code += '<option value="cart_delete">-- '+WH.get_atext('pages.customer.cart_delete')+'</option>';
    code += '<option value="check_out_top_page">-- '+WH.get_atext('pages.customer.check_out_top_page')+'</option>';
    code += '<option value="check_out_popup_cart">-- '+WH.get_atext('pages.customer.check_out_popup_cart')+'</option>';

    //Checkout
    code += '<option value="coupon,gift_card_add,gift_card_delete,change_payment_method,change_freight_delivery,change_customer_type,get_address,place_order">'+WH.get_atext('pages.customer.check_out')+'</option>';
    code += '<option value="coupon">-- '+WH.get_atext('pages.customer.coupon')+'</option>';
    code += '<option value="gift_card_add">-- '+WH.get_atext('pages.customer.gift_card')+'</option>';
    code += '<option value="gift_card_delete">-- '+WH.get_atext('pages.customer.giftcard')+'</option>';
    code += '<option value="change_payment_method">-- '+WH.get_atext('pages.customer.change_payment_method')+'</option>';
    code += '<option value="change_freight_delivery">-- '+WH.get_atext('pages.customer.change_freight_delivery')+'</option>';
    code += '<option value="change_customer_type">-- '+WH.get_atext('pages.customer.change_customer_type')+'</option>';
    code += '<option value="get_address">-- '+WH.get_atext('pages.customer.get_address')+'</option>';
    code += '<option value="place_order">-- '+WH.get_atext('pages.customer.place_order')+'</option>';
    code += '</select>';
    code += '</td>';
    code += '</tr>';
    code += '<tr class="product_search">';
    code += '<td class="text">'+WH.get_atext('pages.customer.ip')+'</td>';
    code += '<td class="text" colspan="2">';
    code += '<input class="text" type="text" name="txtIP" id="txtIP" tabindex="6" />';
    code += '</td>';
    code += '<td class="text">'+WH.get_atext('pages.customer.action_status')+'</td>';
    code += '<td class="text">';
    code += '<select class="text" name="drpActionStatus" tabindex="6">';
    code += '<option value="%" selected="selected">'+WH.get_atext('common.texts.all')+'</option>';
    code += '<option value="1">'+WH.get_atext('pages.customer.success')+'</option>';
    code += '<option value="2">'+WH.get_atext('pages.customer.fail')+'</option>';
    code += '</select>';
    code += '</td>'
    code += '<td></td>'
    code += '</tr>'

    code += '<tr class="text">';
    code += '<td class="text"><br /><div class="small_button_red" onclick="document.frm_search.submit()" tabindex="2">'+WH.get_atext('common.texts.search')+'</div></td>';
    code += '</form>';
    code += '<form name="frm_erase" class="product_search" action="'+Session("base_url")+'admin.asp" method="get">';
    code += '<input type="hidden" name="sp_customeraction" value ="5"/>'
    code += '</form>';
    code += '<td colspan="5"></td>';
    code += '</table>';

    code += "</td></tr></table>";

    return code;
}

//-----------------------------------------------------------------------------
function get_dropdown_cb(tb_name, id_field, text_field, database, selected_id){
  var function_name="get_dropdown_cb",
      i18n = I18N.get_info(), sql_query="", code="";
  try {
    if(database != "system"){
      tb_name = WH.get_tb(tb_name,i18n.current_lang_code);
    }
    sql_query = "select {0} as val, {1} as txt from {2} order by {1}".format([id_field, text_field, tb_name]);
    var rows = wh_db2object2(sql_query, database);
    if(rows && rows.length){
      for(var i=0; i < rows.length; i++){
        code = code + '<option value="{0}"{1}>{2}</option>'.format([rows[i].val, (selected_id==rows[i].val)?' selected="selected"':'',rows[i].txt]);
      }
    }
  }catch(e){
    Log.error(e, function_name);
  }
  return code;
}


function insert_searchcustomer(req){
  var function_name="insert_searchcustomer";
  var code="", page_no=Number(get_session("sp_customerpage"));
  page_no = isNaN(page_no)?1:page_no;
  code = code + '<form name="frm_search" class="product_search" action="'+Session("base_url")+'admin.asp" method="get">';
  code = code + '<input type="hidden" name="sp_customerpage" value ="{0}"/>'.format([get_session("sp_customerpage")]);
  title=WH.get_atext("common.texts.search")+"&nbsp;"+WH.get_atext("common.texts.customer");
  code += '<fieldset class="form-group">';
  code += '<legend>'+title+'</legend>';
  code += '<div class="search-parameters">';
  code = code + '<table class="product_search">';
  code = code + '<tr class="product_search">';
  code = code + '<td class="text right">'+WH.get_atext("common.texts.id")+'</td>';
  code = code + '<td class="text" colspan="2"><input class="form-control" type="text" name="q_customer_id" size="6" value="{0}" tabindex="1"></td>'.format("");
  code = code + '<td class="text right">'+WH.get_atext("pages.customer.first_name")+'</td>';
  code = code + '<td class="text"><input class="form-control" type="text" name="q_customer_firstname" size="30" value="{0}" tabindex="3"></td>'.format("");
  code = code + '<td class="text right">'+WH.get_atext("pages.customer.last_name")+'</td>';
  code = code + '<td class="text"><input class="form-control" type="text" name="q_customer_lastname" size="30" value="{0}" tabindex="3"></td>'.format("");
  code = code + '</tr>';

  code = code + '<tr class="product_search">';
  code = code + '<td class="text right">'+WH.get_atext("common.texts.sort_order")+'</td>';
  code = code + '<td class="text">';
  code = code + '<select class="form-control" name="q_customer_order" tabindex="4">';
  code = code + '<option value="customer_id" selected="selected">{0}</option>'.format([WH.get_atext("pages.customer.customer_id")]);
  code = code + '<option value="last_name">{0}</option>'.format([WH.get_atext("pages.customer.last_name")]);
  code = code + '<option value="first_name">{0}</option>'.format([WH.get_atext("pages.customer.first_name")]);
  code = code + '<option value="city">{0}</option>'.format([WH.get_atext("pages.customer.city")]);
  code = code + '</select>';
  code = code + '</td>';
  code = code + '<td class="text">';
  code = code + '<select class="form-control" name="q_customer_direction" tabindex="4">';
  code = code + '<option value="desc" selected="selected">{0}</option>'.format([WH.get_atext("pages.customer.desc")]);
  code = code + '<option value="asc">{0}</option>'.format([WH.get_atext("pages.customer.asc")]);
  code = code + '</select>';
  code = code + '</td>';
  code = code + '<td class="text right">'+WH.get_atext("pages.customer.company")+'</td>';
  code = code + '<td class="text"><input class="form-control" type="text" name="q_customer_company" size="30" value="{0}" tabindex="3"></td>'.format("");
  code = code + '<td class="text right">'+WH.get_atext("pages.customer.city")+'</td>';
  code = code + '<td class="text"><input class="form-control" type="text" name="q_customer_city" size="30" value="{0}" tabindex="3"></td>'.format("");
  code = code + '</tr>';

  code = code + '<tr class="product_search">';
  code = code + '<td class="text right">'+WH.get_atext("pages.customer.type")+'</td>';
  code = code + '<td class="text" colspan="2">';
  code = code + '<select class="form-control" name="q_customer_type" tabindex="4">';
  code = code + '<option value="%" selected="selected">{0}</option>'.format([WH.get_atext("common.texts.all")]);
  code = code + get_dropdown_cb("alc_customer_type","customer_type_id","customer_type_name","order");
  code = code + '</select>';
  code = code + '</td>';
  code = code + '<td class="text right">'+WH.get_atext("pages.customer.email")+'</td>';
  code = code + '<td class="text"><input class="form-control" type="text" name="q_customer_email" size="30" value="{0}" tabindex="3"></td>'.format("");
  code = code + '<td class="text right">'+WH.get_atext("pages.customer.type_of_business")+'</td>';
  code = code + '<td class="text" colspan="2">';
  code = code + '<select class="form-control" name="type_of_business" tabindex="4">';
  code = code + '<option value="%" selected="selected">{0}</option>'.format([WH.get_atext("common.texts.all")]);
  code = code + get_dropdown_cb("alc_business_type","business_type_id","business_type_name","order");
  code = code + '</select>';
  code = code + '</td>';
  code = code + '</tr>';

  code = code + '<tr class="product_search">';
  code = code + '<td class="text right">'+WH.get_atext("pages.customer.company")+'&nbsp;'+WH.get_atext("pages.customer.type")+'</td>';
  code = code + '<td class="text" colspan="2">';
  code = code + '<select class="form-control" name="q_company_type" tabindex="4">';
  code = code + '<option value="%" selected="selected">{0}</option>'.format([WH.get_atext("common.texts.all")]);
  code = code + get_dropdown_cb("alc_company_type","company_type_id","type_name","order");
  code = code + '</select>';
  code = code + '</td>';
  code = code + '<td class="text right">'+WH.get_atext("common.texts.company_type")+'</td>';
  code = code + '<td class="text">';
  code = code + '<select class="form-control" name="q_company_status" tabindex="4">';
  code = code + '<option value="%">'+WH.get_atext("common.texts.all")+'</option>';
  code = code + get_dropdown_cb("alc_company_status","company_status_id","status_text","order");
  code = code + '</select>';
  code = code + '</td>';
  code = code + '<td class="text right">'+WH.get_atext("pages.customer.business_segment")+'</td>';
  code = code + '<td class="text">';
  code = code + '<select class="form-control" name="business_segment" tabindex="4">';
  code = code + '<option value="%" selected="selected">{0}</option>'.format([WH.get_atext("common.texts.all")]);
  code = code + get_dropdown_cb("alc_business_segment","business_segment_id","business_segment_name","order");
  code = code + '</select>';
  code = code + '</td>';
  code = code + '</tr>';

  code = code + '<tr class="product_search">';
  code = code + '<td class="text right">'+WH.get_atext("pages.customer.sales_responsible")+'</td>';
  code = code + '<td class="text">';
  code = code + '<select class="form-control" name="sales_responsible" tabindex="4">';
  code = code + '<option value="%" selected="selected">{0}</option>'.format([WH.get_atext("common.texts.all")]);
  code = code + get_dropdown_cb("alc_user","user_id","user_name","system");
  code = code + '</select>';

  code = code + '</td>';
  code = code + '</tr>';

  code = code + '<tr class="text">';
  code = code + '<td class="text left"><br />'+search_button(WH.get_atext("common.texts.search"), "javascript:document.frm_search.submit()",2)+'</td>';
  code = code + '<td class="text left"><input class="btn btn-default btn-lg" type="reset" value="Clear"/></td>';
  code = code + '</tr>';
  code = code + '</form>';
  code = code + '</table>';
  code += '</div>';
  code += '</fieldset>';
  return code;
}

function customers2file(){
var function_name="customers2file";
  var code="";

  switch(Session("sp_sendcustomers"))
  {
    case "2":
      Session("sp_sendcustomers")=1;
      if (Session("host_name")!="localhost")
        this_file = "customers.txt";
      else
        this_file = "customers.txt";
      write_customers2file(this_file,new String(Request.Form("sp_tempmail")));
      code=showmenu();
      return code;
      break;
  }

  code = code + '<form name="frm_customers2file" method="post" action="'+Session("base_url")+'admin.asp?sp_sendcustomers=2">';
  title=WH.get_atext('pages.customer.print_customer_list');
  code = code + '<table class="standard"><tr class="standard"><td class="standard">&nbsp;&nbsp;'+title+'</td></tr>';
  code = code + '<tr class="portlet_body"><td class="portlet_body">';

  code = code + '<table class="text">';
  code = code + '<tr class="text">';
  code = code + '<td class="text">';
  code = code + WH.get_atext('common.texts.enter_email')+'<br/><input name="sp_tempmail" value="" size="40"/>';
  code = code + '</td>';

  code = code + '</tr>';
  code = code + '<tr class="text">';
  code = code + '<td class="text">';
  code = code + '<a href="javascript:history.go(-1);"><<&nbsp;'+WH.get_atext('common.texts.back')+'</a>';
  code = code + '</td>';
  code = code + '<td class="text_right">';
  code = code + '<a href="javascript:document.forms.frm_customers2file.submit();">'+WH.get_atext('common.texts.email_me')+'&nbsp;>></a>';
  code = code + '</td>';
  code = code + '</tr>';
  code = code + '</table>';

  code = code + '</td>';
  code = code + '</tr>';
  code = code + '</table>';
  code = code + '</form>';

  return code;
}

function update_customer_detail(name, value, customer_address_id, is_nocheck){
  var function_name="update_customer_detail";
  var conn, rs;
  var sql="", result = false;

  var fieldset_Number  = "discount_level payment_condition";
  try {
    conn= db_openconnection("order");
    rs = Server.CreateObject("ADODB.Recordset");
    if(name == 'customer_address_code'){
      sql="UPDATE alc_customer_address SET is_default = 1 WHERE customer_address_id = " + value + " AND customer_id="+Session("sp_customerid");
      wh_RunSQL(sql,"order");

      sql="UPDATE alc_customer_address SET is_default = 0 WHERE customer_address_id <> " + value + " AND customer_id="+Session("sp_customerid");
      wh_RunSQL(sql,"order");

      sql="UPDATE alc_customer c LEFT JOIN alc_customer_address ca ON ca.customer_id = c.customer_id SET c.address_1 = ca.address_1, c.address_2 = ca.address_2, c.city = ca.city, c.country = ca.country, c.postal_code = ca.postal_code  WHERE customer_address_id = " + value + " AND c.customer_id="+Session("sp_customerid");
      wh_RunSQL(sql,"order");
    }
    else {

      if(name == "title" && value == ""){
        sql="UPDATE alc_customer SET {0} = null WHERE customer_id={1}".format([name, Session("sp_customerid")]);
      }
      else{
        if(name == "country_code") {
          var country = I18N.get_country_name(String(value).toUpperCase());
          sql = "update alc_customer SET {0} = '{1}',country='{3}' WHERE customer_id={2}".format([name, encode_string_todb(value),Session("sp_customerid"),country]);
        } else {
          if (fieldset_Number.indexOf(name)>=0){
            value = isNaN(value)?0:Number(value);
          }
          sql="UPDATE alc_customer SET {0} = '{1}' WHERE customer_id={2}".format([name, encode_string_todb(value),Session("sp_customerid")]);
        }
      }
      wh_RunSQL(sql,"order");
      if(name == "email_work"){
        var e_sha = get_encrypted_email(value);
        wh_RunSQL("UPDATE alc_customer set e_sha = '{0}' WHERE customer_id = {1}".format([e_sha, Session("sp_customerid")]), "order");
      }

      if(customer_address_id > 0 && ",address_1,address_2,postal_code,city,".indexOf(name) > 0){
        sql="UPDATE alc_customer_address SET  " + name + " = '" + encode_string_todb(value) + "' WHERE customer_address_id="+customer_address_id;
        wh_RunSQL(sql,"order");
      }
    }

    if(is_nocheck != "1" && ",email_work,first_name,last_name,phone_fixed".indexOf(name) > 0){
      sql="SELECT contact_id FROM alc_customer_contacts WHERE is_default = 1 AND customer_id="+Session("sp_customerid");
      rs_open(rs,sql,conn);
      if(!rs.EOF){
        if(Number(rs("contact_id")) > 0){
          sql="UPDATE alc_customer_contacts SET  " + name + " = '" + encode_string_todb(value) + "' WHERE contact_id="+rs("contact_id");
          wh_RunSQL(sql,"order");
          Session("contact_id") = Number(rs("contact_id"));
        }
      }
      else {
        var customer = wh_db2object("SELECT first_name, last_name, email_home, phone_fixed FROM alc_customer WHERE customer_id = "+Session("sp_customerid"), conn);
        conn= db_openconnection("order");

        auto_insert_contact_person(customer[0]["first_name"], customer[0]["last_name"], customer[0]["email_work"], customer[0]["phone_fixed"]);
        sql="SELECT contact_id FROM alc_customer_contacts WHERE is_default = 1 AND customer_id="+Session("sp_customerid")+" ORDER BY contact_id DESC LIMIT 1";
        rs_open(rs,sql,conn);
        if(!rs.EOF){
            Session("contact_id") = Number(rs("contact_id"));
        }
      }
    }

    result = true;
  }
  catch (e)
  {
        //save_trace2file(function_name, e.message);
        print_error(e,function_name,sql);
  }
  finally
  {
      db_closeconnection(conn);
      rs_close(rs);
  }
  return result;
}


function update_customertype(cust_type)
{
  var function_name="update_customertype";

  var rs2;
  var sql_query="";

  try {
    var conn;
    conn= db_openconnection("order");

    rs2 = Server.CreateObject("ADODB.Recordset");
    if (remove_null(cust_type) == "") cust_type = 1;

    sql_query="update alc_customer set ";
    sql_query=sql_query+"customer_type="+cust_type;

    sql_query=sql_query+" where customer_id="+Session("sp_customerid");
    rs2.Open(sql_query,conn);

    // change customer type for all orders with this customer

    sql_query="update alc_order_edit set customer_type="+cust_type+" where customer_id="+Session("sp_customerid");
    rs2.open(sql_query,conn);


    db_closeconnection(conn);
  }
  catch (e)
  {
    print_error(e,function_name,sql_query);
        return false;
  }

    return true;
}

function update_companytype(company_type)
{
  var function_name="update_companytype";
  var rs2;
  var sql_query="";

  try {
    var conn;
    conn= db_openconnection("order");

    rs2 = Server.CreateObject("ADODB.Recordset");
    if (remove_null(company_type)=="") company_type=1;

    sql_query="update alc_customer set ";
    sql_query=sql_query+"company_type_id="+company_type;

    sql_query=sql_query+" where customer_id="+Session("sp_customerid");
    rs2.open(sql_query,conn);

    db_closeconnection(conn);

  }
  catch (e)
  {
    print_error(e,function_name,sql_query);
      return false;
  }

  return true;
}


function update_customer_overview()
{
  var function_name="update_customer", sql_query="";
  try {
    row_count=Request.Form("customer_id").Count;

    var i=0;
    while (i<row_count)
    {
      sql_query="update alc_customer set first_name='"+remove_null(encode_string_todb(Request.Form("first_name")(i+1)))+"'";
      sql_query=sql_query+",last_name='"+remove_null(encode_string_todb(Request.Form("last_name")(i+1)))+"'";
      sql_query=sql_query+",address_1='"+remove_null(encode_string_todb(Request.Form("address1")(i+1)))+"'";
      sql_query=sql_query+",company_name='"+remove_null(encode_string_todb(Request.Form("company_name")(i+1)))+"'";
      sql_query=sql_query+",postal_code='"+remove_null(Request.Form("postal_code")(i+1))+"'";
      sql_query=sql_query+",city='"+remove_null(encode_string_todb(Request.Form("city")(i+1)))+"'";
      sql_query=sql_query+",country_code='"+remove_null(encode_string_todb(Request.Form("country_code")(i+1)))+"'";
      sql_query=sql_query+",phone_fixed='"+remove_null(Request.Form("phone_fixed")(i+1))+"'";
      sql_query=sql_query+" where customer_id="+Request.Form("customer_id")(i+1);
      wh_RunSQL(sql_query,"order");

      i++;
    }
  }
  catch (e)
  {
    print_error(e,function_name,sql_query);
  }
}

function delete_customer()
{
var function_name="delete_customer";

  var rs2;
  result = 0;
  var sql_query="";

  try {
  var conn;
  conn= db_openconnection("order");

  rs2 = Server.CreateObject("ADODB.Recordset");

  sql_query="select count(order_id) as count from alc_order_edit where customer_id="+session("sp_customerid");
//response.write("<br />sql="+sql_query);
  rs2.Open(sql_query,conn);

  if (rs2("count")==0)
  {
    rs2.Close();

    sql_query="delete from alc_customer where customer_id="+session("sp_customerid");
//response.write("<br />sql="+sql_query);

    rs2.Open(sql_query,conn);
  }
  else
  {
    result=new Number(rs2("count"));
    rs2.Close();
  }

  delete rs2;
  db_closeconnection(conn);

  }
  catch (e)
  {
    print_error(e,function_name,sql_query);
  }
  return result;
}

function add_customer()
{
var function_name="add_customer";

  var rs2;
  var result = 0;
  var sql_query="";

  try {
    var conn;
    conn= db_openconnection("order");

    rs2 = Server.CreateObject("ADODB.Recordset");

    var next_id = get_customer_nextid();

    var location = String(AppConfig.get_value("system.location"));

    sql_query="update alc_customer set ";
    sql_query = sql_query + "password='24fd05ef4f566475d1d31d2b6c5d6fc7819ca2c440d22157d62d31fc5374dcc7'"; //hardcode password=123Wexthuset to make same webshop that we create customer with a password
    sql_query = sql_query + ",country_code='"+location.toUpperCase()+"'";
    sql_query = sql_query + ",customer_type=1";
    sql_query = sql_query + ",company_type_id=4";
    sql_query = sql_query + ",company_status_id=5";
    sql_query = sql_query + ",confirmed=1";
    sql_query = sql_query + " where customer_id="+next_id;

    rs2.open(sql_query,conn);

    Session("sp_customerid")=next_id;
    Session("q_customer_id")=next_id;
    Session("q_customer_firstname")="";
    Session("q_customer_lastname")="";
    Session("q_customer_order")="last_name";
    Session("q_customer_direction")="DESC";
    Session("q_customer_company")="";
    Session("q_customer_city")="";
    Session("q_customer_email")="";

    db_closeconnection(conn);

  }
  catch (e)
  {
    print_error(e,function_name,sql_query);
  }
  return result;
}


function erase_qfields()
{
  Session("q_customer_id")="";
  Session("q_customer_firstname")="";
  Session("q_customer_lastname")="";
  Session("q_customer_order")="last_name";
  Session("q_customer_direction")="DESC";
  Session("q_customer_company")="";
  Session("q_customer_city")="";
  Session("q_customer_email")="";
}

function customer_address_change_selected(){
   var address_id = 1; //Request.QueryString("address_id");
   //Session("sp_customeraddresscode") = address_id;
}

function approve_reseller_user() {
  var customer_id = Session("sp_customerid"),
      user_id = "", temp_key = "";

  var sql = "SELECT customer_id, reseller_userid, email_work, email_home FROM alc_customer WHERE customer_type=2 and company_type_id=1 and customer_id = "+customer_id;
  var customers = wh_db2object(sql, db_openconnection("order"));
  if(customers && customers.length > 0){
    temp_key = reseller_temp_password(customers[0].customer_id, customers[0].reseller_userid);
    if(temp_key != ""){
        mail_to_reseller(customers[0].email_work, temp_key, customers[0].reseller_userid,customers[0].customer_id );
    }
  }
  return temp_key;
}

function send_activation_link(req)
{
  var function_name="send_activation_link";
  var err_desc = "pos 1";

  sql_query = "SELECT * FROM alc_customer WHERE customer_id = '{0}' ORDER BY customer_id DESC".format([req.customer_id]);
  customers = wh_db2object2(sql_query,"order");
  if(customers && customers.length > 0){
    if(customers[0].e_sha == ""){
      customers[0].e_sha = get_encrypted_email(req.email);
      wh_RunSQL("UPDATE alc_customer set e_sha = '{0}' WHERE customer_id = {1}".format([customers[0].e_sha, req.customer_id]), "order");
    }
    register_confirm(req.email, customers[0].e_sha)
    return true;
  }
  else{
    return false;
  }
}

function send_new_password(req){
  var function_name = "send_new_password", sql_query = "";

  //check condtion before send a new password
  sql_query = "SELECT * FROM alc_customer WHERE customer_id = '{0}' ORDER BY customer_id DESC".format([req.customer_id]);
  customers = wh_db2object2(sql_query,"order");
  if(customers && customers.length > 0){
    if(customers[0].e_sha == ""){
      var e_sha = get_encrypted_email(req.email);
      wh_RunSQL("UPDATE alc_customer set e_sha = '{0}' WHERE customer_id = {1}".format([e_sha, req.customer_id]), "order");
    }

    new_password(req.email, req.customer_id);
    return true;
  }
  else{
    return false;
  }
}

//IF RESELLER FORGOT or NEW APPROVAL
function reseller_temp_password(customer_id, userid)
{
    var temp_salt = "";
    var temp_password = "";
    var temp_password_no_sha = "";
    var temp_password_mix = "";
    var temp_password_width_sha = "";
    var sql = "";
    var result = false;
    var userid_sha = "";
  try
  {
    var conn = db_openconnection("order");
    var rs = Server.CreateObject("ADODB.Recordset");

    //SALT (ALREADY WITH SHA)
    temp_salt = admin_tasty_powder();
    //PASSWORD
    temp_password_no_sha = admin_temp_password();
    //PASSWORD WITH SHA
    temp_password_width_sha = sha256_digest(temp_password_no_sha);
    //MIX PASSWORD WITH SHA AND SALT WITH SHA.
    temp_password_mix = temp_salt + temp_password_width_sha;
    //PASSWORD ADD SHA AGAIN
    temp_password = sha256_digest(temp_password_mix);
    Log.warning( "Temp.Key: "+temp_password_no_sha);
    sql = "SELECT customer_id FROM alc_customer WHERE customer_id = "+sqv(customer_id) + " and reseller_userid = "+sqv(userid)+";";
    rs.open(sql,conn);
    if (!rs.EOF)
    {
      userid_sha = sha256_digest(userid);
      var this_date = new Date();
      this_date = get_datetimestring(this_date);
      sql = "UPDATE alc_customer SET confirmed=2, peppar = "+sqv(temp_salt)+", password ="+sqv(temp_password)+", e_sha ="+sqv(userid_sha)+", password_timestamp ="+sqv(this_date)+" WHERE customer_id = "+sqv(customer_id)+";";
      Log.warning( sql);
      conn.Execute(sql);
    }
    else
    {
      temp_password_no_sha = "";
    }
    rs.close();
    db_closeconnection(conn);
  }
  catch (e)
  {
    db_closeconnection(conn);
    temp_password_no_sha = "";
    Log.error( sql);
  }
  return temp_password_no_sha;
}

function update_sales_responsible(sales_responsible_id){
  var function_name = "update_sales_responsible";
  var sql="", sales_responsible_id, result=false;
  try{
    sql = "update alc_customer set sales_responsible_id = {0} where customer_id = {1}".format([sales_responsible_id,Session("sp_customerid")]);
    sales_responsible_id = wh_RunSQL(sql,"order");
    result = true;
  }catch(e){
    result = false;
    print_error(e, function_name);
  }
  return result;
}
</script>

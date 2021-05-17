/**
 * @file WebVariable.h
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Variable fragments to mount webpage
 * @version 0.1
 * @date 2019-02-09
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef WebVariable_h
#define WebVariable_h

static const char HEADER_EN_US[] PROGMEM = "<!DOCTYPE html><html><head> <meta charset=\"utf-8\"> <meta name=\"viewport\" "
"content=\"width=device-width, initial-scale=1\"> <link rel=\"stylesheet\" href=\"styles.css\" "
"type=\"text/css\"> </head> <body> <nav class=\"navbar navbar-expand-lg navbar-dark bg-primary\">"
" <div class=\"container\"> <a class=\"navbar-brand\" href=\"#\"> <b> ESPSimple</b> </a> "
"<button class=\"navbar-toggler navbar-toggler-right border-0\" type=\"button\" data-toggle=\"collapse\" "
"data-target=\"#navbar17\"> <span class=\"navbar-toggler-icon\"></span> </button> <div class=\"collapse "
"navbar-collapse\" id=\"navbar17\"> <ul class=\"navbar-nav mr-auto\"> <li class=\"nav-item\"> <a class=\"nav-link\" "
"href=\"/\">Home</a> </li> <li class=\"nav-item\" style=\"\"><a class=\"nav-link\" href=\"/wifi.html\">Wi-Fi</a></li> "
"<li class=\"nav-item\"> <a class=\"nav-link\" href=\"/gpio.html\">GPIO</a> </li> <li class=\"nav-item\"> <a class=\"nav-link\""
" href=\"/actions.html\">Actions<br></a> </li> "
#ifdef ESP32
"<li class=\"nav-item\"><a class=\"nav-link\" href=\"adc.html\">ADC</a></li> "
#endif
"<li class=\"nav-item\"><a class=\"nav-link\" href=\"management.html\">Management</a></li> "
#ifdef ENABLE_TELEGRAM
"<li class=\"nav-item\"><a class=\"nav-link\" href=\"telegram.html\">Telegram<br></a></li> "
#endif
"<li class=\"nav-item\"><a class=\"nav-link\" href=\"components.html\">Components</a></li> "
"<li class=\"nav-item\"><a class=\"nav-link\" href=\"pwm.html\">PWM</a></li> "
"<li class=\"nav-item\"><a class=\"nav-link\" href=\"alexa.html\">Alexa</a></li>"
"<li class=\"nav-item\"><a class=\"nav-link\" href=\"/exit.html\">Save and Exit</a></li> "
"</ul> <a class=\"btn btn-outline-light navbar-btn ml-md-2\" target=\"_blank\" "
"href=\"https://github.com/tamusiunas/ESPSimple\" id=\"github.com\">Project Page</a> "
"</div> </div> </nav>";

static const char FOOTER_EN_US_P1[] PROGMEM = "<script src=\"jquery-3.3.1.slim.min.js\"></script> "
"<script src=\"popper.min.js\"></script> <script src=\"bootstrap.min.js\"></script>";

static const char FOOTER_EN_US_P2[] PROGMEM = "</body> </html>";

static const char GPIO_ADC_EN_US_P1[] PROGMEM = "<div class=\"py-5\"> <div class=\"container\"> <div class=\"row\">"
" <div class=\"col-md-12 p-0\"> <div class=\"row\"> <div class=\"col-md-12\"> <h2 class=\"text-center\" "
"style=\"\">ADC (Analog to Digital Converter)</h2> </div> </div> <form method=\"post\" action=\"adcConfig\"> <div "
"class=\"col-md-12 p-3\" style=\"\"> <table class=\"table table-hover "
"table-striped table-bordered\"> <thead class=\"thead-inverse\"> <tr> <th scope=\"col\">GPIO VREF</th> "
"<th scope=\"col\"><select class=\"form-control h-25\" id=\"gpio_adc_vref\" name=\"gpio_adc_vref\"> <option value=\"\">"
"NONE</option>";

static const char GPIO_ADC_EN_US_P2[] PROGMEM = "</select></th> </tr> </thead> </table> <button type=\"submit\" "
"class=\"btn btn-primary my-2\">Submit</button> </div> </form> </div> </div> </div> </div>";

static const char TELEGRAM_JS_EN_US_P1[] PROGMEM = ""
"   <script> $(document).ready(function () { var counter = ";

static const char TELEGRAM_JS_EN_US_P2[] PROGMEM = ""
";\n"
"      $(\"#addrow\").on(\"click\", function () {\n"
"          var newRow = $(\"<tr>\");\n"
"          var cols = \"\";\n"
"          cols += '<td class=\"align-middle\"><label>Message</label><input name=\"telegram_message_r_' + counter + '\" "
"class=\"form-control h-25\" placeholder=\"\" maxlength=\"100\" id=\"telegram_message_r_' + counter + '\"></td>';\n"
"          cols += '<td class=\"align-middle\"> <label>Action</label> <select class=\"form-control h-25\" "
"id=\"telegram_gpio_action_r_' + counter + '\" name=\"telegram_gpio_action_r_' + counter + '\"> <option value=\"reverse\">"
"Reverse Voltage</option> <option value=\"on\">Turn on</option> <option value=\"off\">Turn off</option> </select> "
"<label><br />GPIO target</label> <select class=\"form-control h-25\" id=\"telegram_gpio_target_r_' + counter + '\" "
"name=\"telegram_gpio_target_r_' + counter + '\"> ";

static const char TELEGRAM_JS_EN_US_P3[] PROGMEM = ""
"</select> </td>';\n"
"cols += '<td class=\"align-middle\"><input type=\"button\" class=\"ibtnDel btn btn-primary my-2\"  value=\"Delete\"></td>';\n"
"newRow.append(cols);\n"
"$(\"table.table-telegram-action\").append(newRow);\n"
"counter++;\n"
"});\n"
"$(\"table.table-telegram-action\").on(\"click\", \".ibtnDel\", function (event) {\n"
"$(this).closest(\"tr\").remove();\n"
"});\n"
"});\n"
"</script>\n";

static const char ALEXA_JS_EN_US_P1[] PROGMEM = ""
"   <script> $(document).ready(function () { var counter = ";

static const char ALEXA_JS_EN_US_P2[] PROGMEM = ""
";\n"
"      $(\"#addrow\").on(\"click\", function () {\n"
"          var newRow = $(\"<tr>\");\n"
"          var cols = \"\";\n"
"          cols += '<td class=\"align-middle\"><label>Device Name</label><input name=\"alexa_device_name_r_' + counter + '\" "
"class=\"form-control h-25\" placeholder=\"\" maxlength=\"100\" id=\"alexa_device_name_r_' + counter + '\"></td>';\n"
"          cols += '<td class=\"align-middle\"> <label>Support Dimmer?</label> <select class=\"form-control h-25\" "
"id=\"alexa_support_dimmer_r_' + counter + '\" name=\"alexa_support_dimmer_r_' + counter + '\"> <option value=\"yes\">Yes</option>"
" <option value=\"no\">No</option> </select> <label><br />GPIO target</label> <select class=\"form-control h-25\" "
"id=\"alexa_gpio_target_r_' + counter + '\" name=\"alexa_gpio_target_r_' + counter + '\"> ";

static const char ALEXA_JS_EN_US_P3[] PROGMEM = ""
"</select> </td>';\n"
"cols += '<td class=\"align-middle\"><input type=\"button\" class=\"ibtnDel btn btn-primary my-2\"  value=\"Delete\"></td>';\n"
"newRow.append(cols);\n"
"$(\"table.table-alexa-action\").append(newRow);\n"
"counter++;\n"
"});\n"
"$(\"table.table-alexa-action\").on(\"click\", \".ibtnDel\", function (event) {\n"
"$(this).closest(\"tr\").remove();\n"
"});\n"
"});\n"
"</script>\n";

static const char RESPONSE_OK[] PROGMEM = "<div class=\"py-5\"> <div class=\"container\"> <div class=\"row\"> <div "
"class=\"col-md-12 p-0\"> <div class=\"row\"> <div class=\"col-md-12\"> <h2 class=\"text-center\" style=\"\">"
"{::CONFIG_NAME::} Config Saved!</h2> </div> </div> </div> </div> </div> </div>";

static const char TELEGRAM_EN_US_P1[] PROGMEM = ""
"<form method=\"post\" action=\"/telegramConfig\"> <div class=\"py-5\"> "
"<div class=\"container\"> <div class=\"row\"> <div class=\"col-md-12 p-0\"> <div class=\"row\"> "
"<div class=\"col-md-12\"> <h2 class=\"text-center\" style=\"\">Telegram</h2> </div> </div> <div class=\"col-md-12"
" p-3\" style=\"\"> <table class=\"table table-hover table-striped table-bordered\"> <tbody> <tr> <th scope=\"row\" "
"class=\"align-middle\" style=\"\" >Enable Telegram</th> <td class=\"align-middle\"> "
"<select class=\"form-control h-25\" id=\"telegram_enable\" name=\"telegram_enable\">";

static const char TELEGRAM_EN_US_P2[] PROGMEM = ""
"</select> </td> </tr> <tr> <th scope=\"row\" class=\"align-middle\" "
"style=\"\" >Token ID</th> <td class=\"align-middle\"> <input name=\"telegram_token_id\" "
"class=\"form-control h-25\" placeholder=\"\" maxlength=\"100\" id=\"telegram_token_id\" value=\"";

static const char TELEGRAM_EN_US_P3[] PROGMEM = ""
"\"> </td> </tr> <tr> <th scope=\"row\" class=\"align-middle\" style=\"\" >Client IDs (comma separated)</th> "
"<td class=\"align-middle\"> <input name=\"telegram_client_id\" class=\"form-control h-25\" placeholder=\"\" "
"maxlength=\"200\" id=\"telegram_client_id\" value=\"";

static const char TELEGRAM_EN_US_P4[] PROGMEM = ""
"\"> </td> </tr> </tbody> </table> </div> <div class=\"row\"> <div "
"class=\"col-md-12\"> <h2 class=\"text-center\" style=\"\">Telegram Actions</h2> </div> </div> <div "
"class=\"col-md-12 p-3\" style=\"\"> <table class=\"table table-telegram-action table-hover table-striped "
"table-bordered\"> <tbody>";

static const char TELEGRAM_EN_US_P5[] PROGMEM = ""
"</tbody> <tfoot> <tr> <td colspan=\"5\" style=\"text-align: left;\"> <input "
"type=\"button\" class=\"btn btn-primary my-2 \" id=\"addrow\" value=\"Add Row\" /> </td> </tr> <tr> </tr> "
"</tfoot> </table> <button type=\"submit\" class=\"btn btn-primary my-2\">Submit</button> </div> </div> </div> "
"</div> </div> </form>";

static const char ALEXA_EN_US_P1[] PROGMEM = ""
"<form method=\"post\" action=\"/alexaConfig\"> <div class=\"py-5\"> "
"<div class=\"container\"> <div class=\"row\"> <div class=\"col-md-12 p-0\"> <div class=\"row\"> "
"<div class=\"col-md-12\"> <h2 class=\"text-center\" style=\"\">Alexa</h2> </div> </div> <div class=\"col-md-12"
" p-3\" style=\"\"> <table class=\"table table-hover table-striped table-bordered\"> <tbody> <tr> <th scope=\"row\" "
"class=\"align-middle\" style=\"\" >Enable Alexa</th> <td class=\"align-middle\"> "
"<select class=\"form-control h-25\" id=\"alexa_enable\" name=\"alexa_enable\">";

static const char ALEXA_EN_US_P2[] PROGMEM = ""
"</select> </td> </tr> </tbody> </table> </div> <div class=\"row\"> <div "
"class=\"col-md-12\"> <h2 class=\"text-center\" style=\"\">Alexa Actions</h2> </div> </div> <div "
"class=\"col-md-12 p-3\" style=\"\"> <table class=\"table table-alexa-action table-hover table-striped "
"table-bordered\"> <tbody>";

static const char ALEXA_EN_US_P3[] PROGMEM = ""
"</tbody> <tfoot> <tr> <td colspan=\"5\" style=\"text-align: left;\"> <input "
"type=\"button\" class=\"btn btn-primary my-2 \" id=\"addrow\" value=\"Add Row\" /> </td> </tr> <tr> </tr> "
"</tfoot> </table> <button type=\"submit\" class=\"btn btn-primary my-2\">Submit</button> </div> </div> </div> "
"</div> </div> </form>";

static const char GPIO_EN_US_P1[] PROGMEM = "<div class=\"py-5\"> <div class=\"container\"> <div class=\"row\"> "
"<div class=\"col-md-12 p-0\"> <div class=\"row\"> <div class=\"col-md-12\"> <h2 class=\"text-center\" style=\"\" "
">GPIO Config</h2> </div> </div> <form method=\"post\" action=\"/gpioConfig\"> "
"<div class=\"col-md-12 p-3\" style=\"\"> <table class=\"table table-hover table-striped table-bordered\"> "
"<thead class=\"thead-inverse\"> <tr> <th scope=\"col\">GPIO</th> <th scope=\"col\" "
">Config</th> </tr> </thead> <tbody>";

static const char GPIO_EN_US_P2[] PROGMEM = "</tbody> </table> <button type=\"submit\" class=\"btn "
"btn-primary my-2\">Submit</button> </div> </form> </div> </div> </div> </div>";

static const char PWM_EN_US_P1[] PROGMEM = "<form method=\"post\" action=\"/pwmConfig\"> <div class=\"py-5\"> "
"<div class=\"container\"> <div class=\"row\"> <div class=\"col-md-12 p-0\"> <div class=\"row\"> "
"<div class=\"col-md-12\"> <h2 class=\"text-center\" style=\"\">PWM (Pulse Wave Modulator)</h2> </div> </div> "
#ifndef ESP8266
"<div class=\"col-md-12 p-3\" style=\"\"> "
"<table class=\"table table-hover table-striped table-bordered\" "
"id=\"tablezerocross\"> <tbody> <tr> <th scope=\"row\" class=\"align-middle\" style=\"\" "
">Zero-cross</th> <td class=\"align-middle\"> <label>Enable zero-cross</label> "
"<select class=\"form-control h-25\" id=\"pwm_enable_zero_cross\" name=\"pwm_enable_zero_cross\"> "
"{::SELECTZEROCROSSENABLE::} </select> <label><br>Zero-cross "
"Frequency</label><select class=\"form-control h-25\" id=\"pwm_zero_cross_frequency\" name=\"pwm_zero_cross_frequency\">"
"{::SELECTZEROCROSSFREQUENCY::}"
"</select><label><br>Zero-cross GPIO</label>"
" <select class=\"form-control h-25\" id=\"pwm_zero_cross_gpio\" name=\"pwm_zero_cross_gpio\">";

static const char PWM_EN_US_P2[] PROGMEM = "</select> </td> </tr> </tbody> </table> </div> <div class=\"row\"> "
"<div class=\"col-md-12\"> <h2 class=\"text-center\" style=\"\">PWM Config</h2> </div> </div> "
"<div class=\"col-md-12 p-3\" style=\"\"> <table class=\"table table-zerocross table-hover "
"table-striped table-bordered\" id=\"tablepwm\"> <tbody>";
#endif
#ifdef ESP8266
"";
static const char PWM_EN_US_P2[] PROGMEM = "<div class=\"row\"> "
"<div class=\"col-md-12\"> <h2 class=\"text-center\" style=\"\">PWM Config</h2> </div> </div> "
"<div class=\"col-md-12 p-3\" style=\"\"> <table class=\"table table-zerocross table-hover "
"table-striped table-bordered\" id=\"tablepwm\"> <tbody>";
#endif

static const char PWM_EN_US_P3[] PROGMEM = "</tbody> <tr> <td colspan=\"5\" style=\"text-align: "
"left;\"> <input type=\"button\" class=\"btn btn-primary my-2 \" id=\"addrow\" value=\"Add Row\"> </td> </tr> "
"<tr> </tr> </table> <button type=\"submit\" class=\"btn btn-primary my-2\">Submit</button> </div> </div> </div> "
"</div> </div> </form>";

static const char MANAGEMENT_EN_US_P1[] PROGMEM = "<div class=\"py-5\"> <div class=\"container\"> <div class=\"row\"> "
"<div class=\"col-md-12 p-0\"> <div class=\"row\"> <div class=\"col-md-12\"> <h2 class=\"text-center\" style=\"\">"
"Management</h2> </div> </div> <form method=\"post\" action=\"/managementConfig\"> <div class=\"col-md-12 p-3\" style=\"\"> "
"<table class=\"table table-hover table-striped table-bordered\"> <thead class=\"thead-inverse\"> <tr> "
"<th scope=\"col\">Server</th> <th scope=\"col\">Address</th> </tr> </thead> <tbody> <tr> <th scope=\"row\" "
"class=\"align-middle\" style=\"\" >MQTT</th> <td class=\"align-middle\"><label>IP "
"Address</label> <input name=\"mqtt_ip_address\" class=\"form-control h-25\" placeholder=\"\" maxlength=\"100\" value=\"";

static const char MANAGEMENT_EN_US_P2[] PROGMEM = "\" id=\"mqtt_ip_address\"> <label><br>Port</label> "
"<input name=\"mqtt_port\" class=\"form-control h-25\" "
"placeholder=\"1833\" maxlength=\"10\" id=\"mqtt_port\" type=\"number\" value=\"";

static const char MANAGEMENT_EN_US_P3[] PROGMEM = "\"></td> </tr> <tr> <th scope=\"row\" class=\"align-middle\" "
"style=\"\" >Syslog</th> <td class=\"align-middle\"><label>IP address</label> "
"<input name=\"syslog_ip_address\" class=\"form-control h-25\" placeholder=\"\" maxlength=\"100\" "
"id=\"syslog_ip_address\" value=\"";

static const char MANAGEMENT_EN_US_P4[] PROGMEM = "\"> <label><br>Port</label> <input name=\"syslog_port\" class=\"form-control h-25\" "
"placeholder=\"514\" maxlength=\"10\" id=\"syslog_port\" type=\"number\" value=\"";

static const char MANAGEMENT_EN_US_P5[] PROGMEM = "\"></td> </tr> <tr> <th scope=\"row\" class=\"align-middle\" "
"style=\"\" >OTA</th> <td class=\"align-middle\"><label>Enable OTA</label> <select class=\"form-control h-25\" "
"id=\"enable_ota\" name=\"enable_ota\">";

static const char MANAGEMENT_EN_US_P6[] PROGMEM = "</select> "
"<label><br>OTA password</label> <input name=\"ota_password\" class=\"form-control h-25\" "
"placeholder=\"\" maxlength=\"100\" id=\"ota_password\" value=\"";

static const char MANAGEMENT_EN_US_P7[] PROGMEM = ""
"\"></td> </tr> <tr><th scope=\"row\" class=\"align-middle\" style=\"\" >Web config GPIO</th>  <td class=\"align-middle\">"
"<label>Web config trigger GPIO</label> <select class=\"form-control h-25\" id=\"web_config_gpio\" "
"name=\"web_config_gpio\">";

static const char MANAGEMENT_EN_US_P8[] PROGMEM = "</select>";

static const char MANAGEMENT_EN_US_P9[] PROGMEM = "<label><br />GPIO indicating configuration</label> "
"<select class=\"form-control h-25\" id=\"web_config_indicating\" "
"name=\"web_config_indicating\">";

static const char MANAGEMENT_EN_US_P10[] PROGMEM = "</select><label><br />GPIO indicating configuration Status</label> "
"<select class=\"form-control h-25\" id=\"web_config_indicating_status\" "
"name=\"web_config_indicating_status\">";

static const char MANAGEMENT_EN_US_P11[] PROGMEM = "</select></td></tr></tbody> </table> <button type=\"submit\" "
"class=\"btn btn-primary my-2\">Submit</button> </div> </form> </div> </div> </div> </div>";

static const char WIFI_EN_US_P1[] PROGMEM = "<div class=\"py-5\"> <div class=\"container\"> <div class=\"row\"> <div"
" class=\"col-md-12 p-0\"> <div class=\"row\"> <div class=\"col-md-12\"> <h2 class=\"text-center\" style=\"\">"
"Wi-Fi</h2> </div> </div> <form method=\"post\" action=\"/wifiConfig\"> <div class=\"col-md-12 p-3\" style=\"\"> <table "
"class=\"table table-hover table-striped table-bordered\"> <tbody> <tr> <th scope=\"row\" class=\"align-middle\" "
"style=\"\">Access Point</th> <td class=\"align-middle\"> <label><a href=\"wifi-refresh.html\" "
"class=\"btn btn-primary my-2\">Refresh Networks</a><br />Choose a network</label> <select class=\"form-control "
"h-25\" id=\"access_point_select\" name=\"access_point_select\">";

static const char WIFI_EN_US_P2[] PROGMEM = "</select> <label><br>or enter the network name</label> <input "
"name=\"access_point_manual\" class=\"form-control h-25\" placeholder=\"\" maxlength=\"15\" "
"id=\"access_point_manual\" value=\"";

static const char WIFI_EN_US_P3[] PROGMEM = "\"> </td> </tr> <tr> <th scope=\"row\" class=\"align-middle\" style=\"\" "
">Password</th> <td class=\"align-middle\"> <input name=\"access_point_password\" "
"class=\"form-control h-25\" placeholder=\"\" id=\"access_point_password\" type=\"password\" value=\"";

static const char WIFI_EN_US_P4[] PROGMEM = "\"> </td> </tr> <tr> <th scope=\"row\" "
"class=\"align-middle\" style=\"\" >IP Config (optional)</th> <td class=\"align-middle\">"
" <label>IP address</label> <input name=\"ip_address\" class=\"form-control h-25\" placeholder=\"\" maxlength=\"15\" ";

static const char WIFI_EN_US_P5[] PROGMEM = "id=\"ip_address\" value=\"";

static const char WIFI_EN_US_P6[] PROGMEM = "\"> <label><br>IP mask</label> <input name=\"ip_mask\" "
"class=\"form-control h-25\" placeholder=\"\" "
"maxlength=\"15\" id=\"ip_mask\" value=\"";

static const char WIFI_EN_US_P7[] PROGMEM = "\"> <label><br>Gateway</label> <input name=\"ip_gateway\" class=\"form-control h-25\""
" placeholder=\"\" maxlength=\"15\" id=\"ip_gateway\" value=\"";

static const char WIFI_EN_US_P8[] PROGMEM = "\";> <label><br>DNS</label> <input name=\"ip_dns\" "
"class=\"form-control h-25\" placeholder=\"\" maxlength=\"15\" id=\"ip_dns\"  value=\"";

static const char WIFI_EN_US_P9[] PROGMEM = "\"> </td> "
"</tr> </tbody> </table> <button type=\"submit\" class=\"btn btn-primary my-2\">Submit</button> </div> </form> "
"</div> </div> </div> </div>";

static const char COMPONENTS_EN_US_P1[] PROGMEM = ""
"<form method=\"post\" action=\"/componentsConfig\"> <div class=\"py-5\"> <div class=\"container\"> <div class=\"row\"> "
"<div class=\"col-md-12 p-0\"> <div class=\"row\"> <div class=\"col-md-12\"> <h2 class=\"text-center\" "
"style=\"\">Sensors</h2> </div> </div> <div class=\"col-md-12 p-3\" style=\"\"> <table id=\"table-dht\" "
"class=\"table table-dht table-hover table-striped table-bordered\"> <tbody>";

static const char COMPONENTS_EN_US_P2[] PROGMEM = ""
"</tbody> <tfoot> <tr> <td colspan=\"5\" "
"style=\"text-align: left;\"> <input type=\"button\" class=\"btn btn-primary my-2 \" id=\"addrow-dht\" ";

static const char COMPONENTS_EN_US_P3[] PROGMEM = ""
"value=\"Add DHT(11/22)\"> </td> </tr> <tr> </tr> </tfoot> </table> </div> <div class=\"col-md-12 p-3\" style=\"\"> "
#ifdef ENABLE_BMP180
"<table id=\"table-bmp180\" class=\"table table-bmp180 table-hover table-striped table-bordered\"> <tbody>"
#endif
"";

static const char COMPONENTS_EN_US_P4[] PROGMEM = ""
#ifdef ENABLE_BMP180
"</tbody> "
"<tfoot> <tr> <td colspan=\"5\" style=\"text-align: left;\"> <input type=\"button\" class=\"btn btn-primary my-2 \" "
"id=\"addrow-bmp180\" value=\"Add BMP180\"> </td> </tr> <tr> </tr> </tfoot> </table> </div> "
#endif
#ifdef ENABLE_MCP3008
"<div class=\"col-md-12 p-3\" style=\"\"> <table id=\"table-mcp3008\" class=\"table table-mcp3008 table-hover "
"table-striped table-bordered\"> <tbody>";
#endif
"";

static const char COMPONENTS_EN_US_P5[] PROGMEM = ""
#ifdef ENABLE_MCP3008
"</tbody> <tfoot> <tr> <td colspan=\"5\" style=\"text-align: left;\"> "
"<input type=\"button\" class=\"btn btn-primary my-2 \" id=\"addrow-mcp3008\" value=\"Add MCP3008\"> </td> </tr> "
#endif
"";

static const char COMPONENTS_EN_US_P6[] PROGMEM = ""
#ifdef ENABLE_MCP3008
"<tr> </tr> </tfoot> </table> </div> "
#endif
#ifdef ENABLE_SSD160
"<div class=\"row\"> <div class=\"col-md-12\"> <h2 class=\"text-center\" "
"style=\"\">Displays</h2> </div> </div> <div class=\"col-md-12 p-3\" style=\"\"> <table id=\"table-ssd160\" "
"class=\"table table-ssd160 table-hover table-striped table-bordered\"> <tbody>"
#endif
"";

static const char COMPONENTS_EN_US_P7[] PROGMEM = ""
#ifdef ENABLE_SSD160
"</tbody> <tfoot> <tr> <td colspan=\"5\""
"style=\"text-align: left;\"> <input type=\"button\" class=\"btn btn-primary my-2 \" id=\"addrow-ssd160\" "
"value=\"Add SSD160\"> </td> </tr> <tr> </tr> </tfoot> </table> </div> <div class=\"col-md-12 p-3\" style=\"\"> "
"<table id=\"table-ssd160-frames\" class=\"table table-ssd160-frames table-hover table-striped table-bordered\"> "
"<tbody>"
#endif
"";

static const char COMPONENTS_EN_US_P8[] PROGMEM = ""
#ifdef ENABLE_SSD160
"</tbody> <tfoot> <tr> <td colspan=\"5\" style=\"text-align: left;\"> <input type=\"button\" "
"class=\"btn btn-primary my-2 \" id=\"addrow-ssd160-frames\" value=\"Add SSD160 Frame\"> </td> </tr> <tr> </tr> "
"</tfoot> </table>"
#endif
"<button type=\"submit\" class=\"btn btn-primary my-2\">Submit</button></div> </div> </div> </div>"
" </div> </form>"
"";

static const char COMPONENTS_JS_EN_US_P1[] PROGMEM = ""
"<script>\n"
"$(document).ready(function () {\n"
"var counter_dht = ";

static const char COMPONENTS_JS_EN_US_P2[] PROGMEM = ""
";\n"
"var counter_bmp180 = ";

static const char COMPONENTS_JS_EN_US_P3[] PROGMEM = ""
";\n"
"var counter_mcp3008 = ";

static const char COMPONENTS_JS_EN_US_P4[] PROGMEM = ""
";\n"
"var counter_ssd160 = ";

static const char COMPONENTS_JS_EN_US_P5[] PROGMEM = ""
";\n"
"var counter_ssd160_frames = ";

static const char COMPONENTS_JS_EN_US_P6[] PROGMEM = ""
";\n"
"if (counter_dht > 1)\n"
"{\n"
"$(\"#addrow-dht\").attr(\"disabled\",true);\n"
"}\n"
"if (counter_bmp180 > 0)\n"
"{\n"
"$(\"#addrow-bmp180\").attr(\"disabled\",true);\n"
"}\n"
"if (counter_mcp3008 > 0)\n"
"{\n"
"$(\"#addrow-mcp3008\").attr(\"disabled\",true);\n"
"}\n"
"if (counter_ssd160 > 0)\n"
"{\n"
"$(\"#addrow-ssd160\").attr(\"disabled\",true);\n"
"$(\"#table-ssd160-frames\").removeClass(\"collapse\");\n"
"}\n"
"else\n"
"{\n"
"$('#table-ssd160-frames').addClass(\"collapse\");\n"
"}\n"
"$(\"#addrow-dht\").on(\"click\", function () {\n"
"var newRow = $(\"<tr>\");\n"
"var cols_digital = \"\";\n"
"cols_digital += '<th scope=\"row\" class=\"align-middle\" style=\"\">DHT (11/22)</th>';\n"
"cols_digital += '<td class=\"align-middle\"><label>DHT Type</label><select "
"class=\"form-control h-25\" id=\"component_dht_type_' + counter_dht + '\" name=\"component_dht_type_' + counter_dht + '\""
"><option value=\"dht11\">DHT11</option><option value=\"dht22\">DHT22</option></select>"
"<label><br />DHT GPIO</label><select class=\"form-control h-25\" "
"id=\"component_dht_gpio_' + counter_dht + '\" name=\"component_dht_gpio_' + counter_dht + '\">";

static const char COMPONENTS_JS_EN_US_P7[] PROGMEM = ""
"</select> </td>';\n"
"cols_digital += '<td class=\"align-middle\"><input type=\"button\" class=\"ibtnDel-dht btn btn-primary my-2\"  value=\"Delete\"></td>';\n"
"newRow.append(cols_digital);\n"
"$(\"table.table-dht\").append(newRow);\n"
"counter_dht++;\n"
"if (counter_dht > 1)\n"
"{\n"
"$(this).attr(\"disabled\",true);\n"
"}\n"
"});\n";

static const char COMPONENTS_JS_EN_US_P8[] PROGMEM = ""
"$(\"table.table-dht\").on(\"click\", \".ibtnDel-dht\", function (event) {\n"
"$(this).closest(\"tr\").remove();\n"
"$(\"#addrow-dht\").attr(\"disabled\",false);\n"
"});\n"
"$(\"#addrow-bmp180\").on(\"click\", function () {\n"
"var newRow = $(\"<tr>\");\n"
"var cols_digital = \"\";\n"
"cols_digital += '<th scope=\"row\" class=\"align-middle\" style=\"\">BMP180</th>';\n"
"cols_digital += '<td class=\"align-middle\"><label>BMP180 SDA</label> <select class=\"form-control h-25\" "
"id=\"component_bmp180_sda_' + counter_bmp180 + '\" name=\"component_bmp180_sda_' + counter_bmp180 + '\">";

static const char COMPONENTS_JS_EN_US_P9[] PROGMEM = ""
"</select><label><br>BMP180 SCL</label> <select class=\"form-control h-25\" id=\"component_bmp180_scl_' + counter_bmp180 + '\" "
"name=\"component_bmp180_scl_' + counter_bmp180 + '\">";

static const char COMPONENTS_JS_EN_US_P10[] PROGMEM = ""
"</select></td>';\n"
"cols_digital += '<td class=\"align-middle\"><input type=\"button\" class=\"ibtnDel-bmp180 btn btn-primary my-2\"  "
"value=\"Delete\"></td>';\n"
"newRow.append(cols_digital);\n"
"$(\"table.table-bmp180\").append(newRow);\n"
"counter_bmp180++;\n"
"if (counter_bmp180 > 0)\n"
"{\n"
"$(this).attr(\"disabled\",true);\n"
"}\n"
"});\n";

static const char COMPONENTS_JS_EN_US_P11[] PROGMEM = ""
"$(\"table.table-bmp180\").on(\"click\", \".ibtnDel-bmp180\", function (event) {\n"
"$(this).closest(\"tr\").remove();\n"
"$(\"#addrow-bmp180\").attr(\"disabled\",false);\n"
"});\n"
"$(\"#addrow-mcp3008\").on(\"click\", function () {\n"
"var newRow = $(\"<tr>\");\n"
"var cols_digital = \"\";\n"
"cols_digital += '<th scope=\"row\" class=\"align-middle\" style=\"\">MCP3008</th>';\n"
"cols_digital += '<td class=\"align-middle\"><label>MCP3008 Clock GPIO</label> <select class=\"form-control h-25\" "
"id=\"component_mcp3008_clock_' + counter_mcp3008 + '\" name=\"component_mcp3008_clock_' + counter_mcp3008 + '\"> ";

static const char COMPONENTS_JS_EN_US_P12[] PROGMEM = ""
"</select> <label><br>MCP3008 MISO GPIO</label> <select class=\"form-control h-25\" id=\"component_mcp3008_miso_' + "
"counter_mcp3008 + '\" name=\"component_mcp3008_miso_' + counter_mcp3008 + '\">";

static const char COMPONENTS_JS_EN_US_P13[] PROGMEM = ""
"</select> <label><br>MCP3008 MOSI GPIO</label> <select class=\"form-control h-25\" id=\"component_mcp3008_mosi_' + "
"counter_mcp3008 + '\" name=\"component_mcp3008_mosi_' + counter_mcp3008 + '\">";

static const char COMPONENTS_JS_EN_US_P14[] PROGMEM = ""
"</select> <label><br>MCP3008 CS GPIO</label> <select class=\"form-control h-25\" id=\"component_mcp3008_cs_' + counter_mcp3008"
" + '\" name=\"component_mcp3008_cs_' + counter_mcp3008 + '\">";

static const char COMPONENTS_JS_EN_US_P15[] PROGMEM = ""
"</select></td>';\n"
"cols_digital += '<td class=\"align-middle\"><input type=\"button\" class=\"ibtnDel-mcp3008 btn btn-primary my-2\"  value=\"Delete\"></td>';\n"
"newRow.append(cols_digital);\n"
"$(\"table.table-mcp3008\").append(newRow);\n"
"counter_mcp3008++;\n"
"if (counter_mcp3008 > 0)\n"
"{\n"
"$(this).attr(\"disabled\",true);\n"
"}\n"
"});\n"
"$(\"table.table-mcp3008\").on(\"click\", \".ibtnDel-mcp3008\", function (event) {\n";

static const char COMPONENTS_JS_EN_US_P16[] PROGMEM = ""
"$(this).closest(\"tr\").remove();\n"
"$(\"#addrow-mcp3008\").attr(\"disabled\",false);\n"
"});\n"
"$(\"#addrow-ssd160\").on(\"click\", function () {\n"
"var newRow = $(\"<tr>\");\n"
"var cols_digital = \"\";\n"
"cols_digital += '<th scope=\"row\" class=\"align-middle\" style=\"\">SSD160</th>';\n"
"cols_digital += '<td class=\"align-middle\"><label>SDA</label> <select class=\"form-control h-25\" "
"id=\"component_ssd160_sda_' + counter_ssd160 + '\" name=\"component_ssd160_sda_' + counter_ssd160 + '\">";

static const char COMPONENTS_JS_EN_US_P17[] PROGMEM = ""
"</select> <label><br>SCL</label> <select class=\"form-control h-25\" id=\"component_ssd160_scl_' + counter_ssd160 + '\" "
"name=\"component_ssd160_scl_' + counter_ssd160 + '\">";

static const char COMPONENTS_JS_EN_US_P18[] PROGMEM = ""
"</select></td>';\n"
"cols_digital += '<td class=\"align-middle\"><input type=\"button\" class=\"ibtnDel-ssd160 btn btn-primary my-2\"  value=\"Delete\"></td>';\n"
"newRow.append(cols_digital);\n"
"$(\"table.table-ssd160\").append(newRow);\n"
"counter_ssd160++;\n"
"if (counter_ssd160 > 0)\n"
"{\n"
"$(\"#table-ssd160-frames\").removeClass(\"collapse\");\n"
"$(this).attr(\"disabled\",true);\n"
"}\n"
"});\n"
"$(\"table.table-ssd160\").on(\"click\", \".ibtnDel-ssd160\", function (event) {\n"
"$(this).closest(\"tr\").remove();\n";

static const char COMPONENTS_JS_EN_US_P19[] PROGMEM = ""
"$(\"#table-ssd160-frames\").addClass(\"collapse\");\n"
"$(\"#addrow-ssd160\").attr(\"disabled\",false);\n"
"});\n"
"$(\"#addrow-ssd160-frames\").on(\"click\", function () {\n"
"var newRow = $(\"<tr>\");\n"
"var cols_digital = \"\";\n"
"cols_digital += '<th scope=\"row\" class=\"align-middle\" style=\"\">Frame</th>';\n"
"cols_digital += '<td class=\"align-middle\"><label>Frame Type</label> <select class=\"form-control h-25\" "
" id=\"component_ssd160_frame_type_' + counter_ssd160_frames + '\" name=\"component_ssd160_frame_type_' + counter_ssd160_frames + '\"> "
"<option value=\"mqtt\">MQTT</option> <option value=\"ota\">Over The Air (OTA) Update</option></select><label><br />"
"Delay</label><input name=\"component_ssd160_frame_delay_' + counter_ssd160_frames + '\" type=\"number\" "
"class=\"form-control h-25\" placeholder=\"\" maxlength=\"10\" id=\"component_ssd160_frame_delay_' + counter_ssd160_frames + ";

static const char COMPONENTS_JS_EN_US_P20[] PROGMEM = ""
"'\"></td>';\n"
"cols_digital += '<td class=\"align-middle\"><input type=\"button\" class=\"ibtnDel-ssd160-frames btn btn-primary "
"my-2\"  value=\"Delete\"></td>';\n"
"newRow.append(cols_digital);\n"
"$(\"table.table-ssd160-frames\").append(newRow);\n"
"counter_ssd160_frames++;\n"
"});\n"
"$(\"table.table-ssd160-frames\").on(\"click\", \".ibtnDel-ssd160-frames\", function (event) {\n"
"$(this).closest(\"tr\").remove();\n"
"});\n"
"});\n"
"</script>";

static const char ACTIONS_EN_US_P1[] PROGMEM = ""
"<form method=\"post\" action=\"/actionsConfig\"> <div class=\"py-5\"> "
"<div class=\"container\"> <div class=\"row\"> <div class=\"col-md-12 p-0\"> <div class=\"row\"> "
"<div class=\"col-md-12\"> <h2 class=\"text-center\" style=\"\">Actions (Digital)</h2> </div> </div> "
"<div class=\"col-md-12 p-3\" style=\"\"> <table class=\"table table-actions-digital table-hover table-striped "
"table-bordered\"> <tbody>";

static const char ACTIONS_EN_US_P2[] PROGMEM = ""
"</tbody> <tfoot> <tr> <td colspan=\"5\" style=\"text-align: left;\"> <input type=\"button\" "
"class=\"btn btn-primary my-2 \" id=\"addrow-digital\" value=\"Add Action (Digital)\"> </td> </tr> <tr> </tr> </tfoot>";

static const char ACTIONS_EN_US_P3[] PROGMEM = ""
" </table> </div> <div class=\"row\"> <div class=\"col-md-12\"> <h2 class=\"text-center\" style=\"\">Actions "
"(ADC)</h2> </div> </div> <div class=\"col-md-12 p-3\" style=\"\"> <table class=\"table table-actions-adc table-hover "
"table-striped table-bordered\"> <tbody>";

static const char ACTIONS_EN_US_P4[] PROGMEM = ""
"</tbody> <tfoot> <tr> <td colspan=\"5\" style=\"text-align: left;\"> "
"<input type=\"button\" class=\"btn btn-primary my-2 \" id=\"addrow-adc\" value=\"Add Action (ADC)\"> </td> </tr> <tr>"
" </tr> </tfoot> </table> <button type=\"submit\" class=\"btn btn-primary my-2\">Submit</button> </div> </div> </div>"
" </div> </div> </form>";

static const char ACTIONS_JS_EN_US_P1[] PROGMEM = ""
"<script src=\"jquery-3.3.1.slim.min.js\"></script> <script src=\"popper.min.js\"></script> <script "
"src=\"bootstrap.min.js\"></script><script>\n"
"$(document).ready(function () {\n"
"var counter_digital = {::COUNTERDIGITAL::};\n"
"var counter_adc = {::COUNTERADC::};\n";

static const char ACTIONS_JS_EN_US_P2[] PROGMEM = ""
"$(\"#addrow-digital\").on(\"click\", function () {\n"
"var newRow = $(\"<tr>\");\n"
"var cols_digital = \"\";\n"
"cols_digital += '<td scope=\"row\" class=\"align-middle\" style=\"\"><label>GPIO</label><select "
"class=\"form-control h-25\" id=\"action_digital_gpio_origin_r_' + counter_digital + '\" name=\"action_digital_gpio_origin_r_' +"
" counter_digital + '\">";

static const char ACTIONS_JS_EN_US_P3[] PROGMEM = ""
"</select></td>';\n"
"cols_digital += '<td class=\"align-middle\"> <label>On event</label> "
"<select class=\"form-control h-25\" id=\"action_digital_trigger_analisis_type_r_' + counter_digital + '\" "
"name=\"action_digital_trigger_analisis_type_r_' + counter_digital + '\"> <option value=\"on\">On</option> "
"<option value=\"off\">Off</option> <option value=\"reversed\">Reversed</option> </select> <label><br />"
"Action</label> <select class=\"form-control h-25\" id=\"action_digital_action_r_' + counter_digital + "
"'\" name=\"action_digital_action_r_' + counter_digital + '\"> <option value=\"reverse\">Reverse Voltage</option> "
"<option value=\"on\">Turn on</option> <option value=\"off\">Turn off</option> <option "
"value=\"sendmessagetelegram\">Send Telegram Message</option></select>" 
"<label><br>Time before action reversal (ms)</label><input name=\"action_digital_time_before_action_reversal_r_' + counter_digital + '\""
"class=\"form-control h-25\" placeholder=\"\" maxlength=\"10\" type=\"number\" id=\"action_digital_time_before_action_reversal_r_' + counter_digital + "
"'\"  value=\"\">"
"<label><br />GPIO target</label> "
"<select class=\"form-control h-25\" id=\"action_digital_gpio_target_r_' + counter_digital + '\" "
"name=\"action_digital_gpio_target_r_' + counter_digital + '\">";

static const char ACTIONS_JS_EN_US_P4[] PROGMEM = ""
"</select><label><br />Telegram message (output)</label><input name=\"action_digital_telegram_message_r_' + counter_digital + '\" "
"class=\"form-control h-25\" placeholder=\"\" maxlength=\"100\" id=\"action_digital_telegram_message_r_' + counter_digital "
"+ '\"><label><br />Waiting time before rearming trigger (ms)</label><input name=\"action_digital_waiting_time_rearm_r_' + "
"counter_digital + '\" type=\"number\" class=\"form-control h-25\" value=0 placeholder=\"\" maxlength=\"5\" "
"id=\"action_digital_waiting_time_rearm_r_' + counter_digital + '\">  </td>';\n"
"cols_digital += '<td class=\"align-middle\"><input type=\"button\" class=\"ibtnDel-digital btn btn-primary "
"my-2\"  value=\"Delete\"></td>';\n"
"newRow.append(cols_digital);\n"
"$(\"table.table-actions-digital\").append(newRow);\n"
"counter_digital++;\n"
"});\n"
"$(\"table.table-actions-digital\").on(\"click\", \".ibtnDel-digital\", function (event) {\n"
"$(this).closest(\"tr\").remove();\n"
"});\n"
"$(\"#addrow-adc\").on(\"click\", function () {\n"
"var newRow = $(\"<tr>\");\n"
"var cols_adc = \"\";\n"
"cols_adc += '<td scope=\"row\" class=\"align-middle\" style=\"\"><label>GPIO</label><select "
"class=\"form-control h-25\" id=\"action_adc_gpio_origin_r_' + counter_adc + '\" name=\"action_adc_gpio_origin_r_' + "
"counter_adc + '\">";

static const char ACTIONS_JS_EN_US_P5[] PROGMEM = ""
"</select></td>';\n"
"cols_adc += '<td class=\"align-middle\"> <label>On event</label> <select class=\"form-control h-25\" "
"id=\"action_adc_trigger_analisis_type_r_' + counter_adc + '\" name=\"action_adc_trigger_analisis_type_r_' + counter_adc + '\"> "
"<option value=\"variation\">Variation</option> <option value=\"greaterthan\">Greater Than</option> "
"<option value=\"lowerthan\">Lower Than</option> </select> <label><br />Value</label><input "
"name=\"action_adc_trigger_value_r_' + counter_adc + '\" type=\"number\" class=\"form-control h-25\" placeholder=\"\" "
"maxlength=\"5\" id=\"action_adc_trigger_value_r_' + counter_adc + '\"><label><br />Action</label> <select "
"class=\"form-control h-25\" id=\"action_adc_action_r_' + counter_adc + '\" name=\"action_adc_action_r_' + counter_adc + '\"> "
"<option value=\"reverse\">Reverse Voltage</option> <option value=\"on\">Turn on</option> <option value=\"off\">Turn "
"off</option> <option value=\"sendmessagetelegram\">Send Telegram Message</option></select> <label><br />Time Before Action "
"Reversal</label><input name=\"action_adc_time_before_action_reversal_r_' + counter_adc + '\" class=\"form-control h-25\" "
"placeholder=\"\" maxlength=\"10\" type=\"number\" id=\"action_adc_time_before_action_reversal_r_' + counter_adc + '\"> <label><br />"
"GPIO target</label> <select class=\"form-control h-25\" id=\"action_adc_gpio_target_r_' + counter_adc + '\" "
"name=\"action_adc_gpio_target_r_' + counter_adc + '\">";

static const char ACTIONS_JS_EN_US_P6[] PROGMEM = ""
"</select><label><br />Telegram message (output)</label><input name=\"action_adc_telegram_message_r_' + counter_adc + '\" "
"class=\"form-control h-25\" placeholder=\"\" maxlength=\"100\" id=\"action_adc_telegram_message_r_' + counter_adc + '\">"
"<label><br />Waiting time before rearming trigger (ms)</label><input name=\"action_adc_waiting_time_rearm_r_' + counter_adc"
" + '\" type=\"number\" class=\"form-control h-25\" value=0 placeholder=\"\" maxlength=\"5\" "
"id=\"action_adc_waiting_time_rearm_r_' + counter_adc + '\">  </td>';\n"
"         cols_adc += '<td class=\"align-middle\"><input type=\"button\" class=\"ibtnDel-adc btn btn-primary my-2\"  "
"value=\"Delete\"></td>';\n"
"         newRow.append(cols_adc);\n"
"             $(\"table.table-actions-adc\").append(newRow);\n"
"             counter_adc++;\n"
"         });\n"
"         $(\"table.table-actions-adc\").on(\"click\", \".ibtnDel-adc\", function (event) {\n"
"             $(this).closest(\"tr\").remove();\n"
"         });\n"
"         });\n"
"      </script>\n";

static const char PWM_JS_EN_US_P1[] PROGMEM = ""
"<script>\n"
"$(document).ready(function () {\n"
"var counter = ";

static const char PWM_JS_EN_US_P2[] PROGMEM = ""
";\n"
"$(\"#addrow\").on(\"click\", function () {\n"
"var newRow = $(\"<tr>\");\n"
"var cols = \"\";\n"
"cols += '<td class=\"align-middle\"> <label>GPIO</label><select class=\"form-control h-25\" "
"id=\"pwm_gpio_r_' + counter + '\" name=\"pwm_gpio_r_' + counter + '\">";

static const char PWM_JS_EN_US_P3[] PROGMEM = ""
"</select> </td>';\n"
"cols += '<td class=\"align-middle\"> <label>PWM Source</label><select class=\"form-control h-25\" "
"id=\"pwm_source_r_' + counter + '\" name=\"pwm_source_r_' + counter + '\">"
#ifndef ESP8266
"<option value=\"zerocross\">Based on zero-cross</option>"
#endif
"<option value=\"nonzerocross\">Do not use Zero-cross</option>"
"</select></td>';\n"
"cols += '<td class=\"align-middle\"><input type=\"button\" class=\"ibtnDel btn btn-primary my-2\"  "
"value=\"Delete\"></td>';\n"
"newRow.append(cols);\n"
"$(\"table.table-zerocross\").append(newRow);\n"
"counter++;\n"
"});\n"
"$(\"table.table-zerocross\").on(\"click\", \".ibtnDel\", function (event) {\n"
"$(this).closest(\"tr\").remove();\n"
"});\n"
"});\n"
"</script>\n";



// http
const char HTTP_HEAD_CL[] PROGMEM = "Content-Length";
const char HTTP_HEAD_CT_HTML[] PROGMEM = "text/html";
const char HTTP_HEAD_CT_CSS[] PROGMEM = "text/css";
const char HTTP_HEAD_CT_JS[] PROGMEM = "text/javascript";
const char HTTP_HEAD_CT_TP[] PROGMEM = "text/plain";


 #endif

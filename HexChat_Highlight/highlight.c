#define DEBUG_MSGS 1

///
/// Much of the logic that is actually in use has beedn adapted from TingPing's phyton implimentation 
/// https://github.com/TingPing/plugins/blob/master/HexChat/highlight.py
///

#include "hexchat-plugin.h"
#include <Windows.h>

#define PNAME "Highlight Window"
#define PDESC "A simple plugin to show Highlights in a seperate window"
#define PVERSION "0.3.1"

static hexchat_plugin *ph;
static char *TAB_NAME = "hilights";
#ifndef _DEBUG
static int iHighlightCounter = 0;
#endif // End !_DEBUG check
#ifndef DEBUG_MSGS
const char **newtofrontstr;
#endif //End !DEBUG_MSG check

hexchat_context *find_HL_Tab (){
	hexchat_context *context;
	context = hexchat_find_context (ph, NULL, TAB_NAME);
	if (context == NULL)
	{
#ifndef DEBUG_MSGS
		int newtofrontint;
		int preftype = hexchat_get_prefs(ph, "gui_tab_newtofront",newtofrontstr, &newtofrontint);
		hexchat_command(ph, "set -quiet gui_tab_newtofront 0");
		hexchat_commandf(ph, "newserver -noconnect %s", TAB_NAME);
		if (preftype == 1)
			hexchat_commandf(ph, "set -quiet gui_tab_newtofront %s", newtofrontstr);
		else
			hexchat_commandf(ph, "set -quiet gui_tab_newtofront %d", newtofrontint);
#endif //End !DEBUG_MSG check

		hexchat_commandf(ph, "query -nofocus %s", TAB_NAME);

		return hexchat_find_context (ph, NULL, TAB_NAME);

	}
	else
	{
		return context;
	}
}

static int print_cb (char *word[], 
					 void *userdata)
{	
	hexchat_context *HLcontext = find_HL_Tab();
	hexchat_context *CurContext = hexchat_find_context(ph,NULL,NULL);
	char *channel = (char *)hexchat_get_info(ph, "channel");
#ifndef _DEBUG
	hexchat_context *targetChannelConext = hexchat_find_context(ph, NULL, channel);
#endif // !_DEBUG
	hexchat_set_context(ph, HLcontext);

	hexchat_printf(ph, "%s: <%s%s> %s", channel, word[3],word[1],word[2]);
#ifndef _DEBUG
	if (iHighlightCounter == 1)
	{
		hexchat_commandf(ph, "PRIVMSG %s :I HURD U TALKIN SHIT", channel);
		hexchat_set_context(ph, targetChannelConext);
		hexchat_emit_print(ph, "Your Message", "shevolt", "I HURD U TALKIN SHIT", "", "");
		hexchat_set_context(ph, CurContext);
	}
	else if (iHighlightCounter == 49)
	{
		iHighlightCounter = 0;
		hexchat_set_context(ph, CurContext);
	}

	iHighlightCounter++;
#else
	hexchat_set_context(ph, CurContext);
#endif //End !_DEBUG check

	return HEXCHAT_EAT_NONE;

}


void hexchat_plugin_get_info (char **name, 
							  char **desc, 
							  char **version, 
							  void **reserved)
{

	*name = PNAME;
	*desc = PDESC;
	*version = PVERSION;

}

int hexchat_plugin_init (hexchat_plugin *plugin_handle, 
						 char **plugin_name, 
						 char **plugin_desc, 
						 char **plugin_version, 
						 char *arg)
{

	ph = plugin_handle;

	*plugin_name = PNAME;
	*plugin_desc = PDESC;
	*plugin_version = PVERSION;

	hexchat_hook_print(ph,"Channel Msg Hilight", HEXCHAT_PRI_NORM,print_cb,"Channel Msg Hilight");
	hexchat_hook_print(ph,"Channel Action Hilight", HEXCHAT_PRI_NORM,print_cb,"Channel Action Hilight");
	hexchat_print(ph, "Hightlight plugin loaded\r\n");

	return 1;

}

int hexchat_plugin_deinit (hexchat_plugin *plugin_handle)
{
	hexchat_printf (plugin_handle, "%s plugin unloaded.\r\n", PNAME);
	return 1;
}
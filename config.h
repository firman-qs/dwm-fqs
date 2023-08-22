/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
#define SESSION_FILE "/tmp/dwm-session"

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {"st", "-n", "spfm", "-g", "144x41", "-e", "ranger", NULL };
const char *spcmd3[] = {"keepassxc", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spranger",    spcmd2},
	{"keepassxc",   spcmd3},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",	  NULL,			NULL,		0,				1,			 -1 },
	{ "Firefox",  NULL,			NULL,		1 << 8,			0,			 -1 },
	{ NULL,		  "spterm",		NULL,		SPTAG(0),		1,			 -1 },
	{ NULL,		  "spfm",		NULL,		SPTAG(1),		1,			 -1 },
	{ NULL,		  "keepassxc",	NULL,		SPTAG(2),		0,			 -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALTKEY Mod1Mask
#define TAGKEYS(KEY,TAG)												\
	&((Keychord){1, {{MODKEY, KEY}},								view,           {.ui = 1 << TAG} }), \
		&((Keychord){1, {{MODKEY|ControlMask, KEY}},					toggleview,     {.ui = 1 << TAG} }), \
		&((Keychord){1, {{MODKEY|ShiftMask, KEY}},						tag,            {.ui = 1 << TAG} }), \
		&((Keychord){1, {{MODKEY|ControlMask|ShiftMask, KEY}},			toggletag,      {.ui = 1 << TAG} }),

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };

static Keychord *keychords[] = {
	/*-------------------------------------------------------------------------------------------------------------*/
	/*  n-sequence  |  keychord-modifier-key 				|      function    		|            argument          */
	/*-------------------------------------------------------------------------------------------------------------*/
	// APPS
	&((Keychord){1, { {MODKEY, XK_Return}					},	spawn,			{.v = termcmd } }),
	&((Keychord){1, { {MODKEY, XK_b}						},	spawn,			SHCMD("firefox") }),
	&((Keychord){1, { {MODKEY, XK_e}						},	spawn,			SHCMD("nemo") }),
	&((Keychord){1, { {MODKEY, XK_s}						},	spawn,			SHCMD("flameshot gui") }),

	// DWM
	&((Keychord){1, { {MODKEY, XK_x}						},	spawn,			SHCMD("dm-logout") }),
	&((Keychord){1, { {MODKEY|ShiftMask, XK_b}				},  togglebar,		{0} }),
	&((Keychord){1, { {MODKEY|ShiftMask, XK_q}				},  quit,			{0} }),
	&((Keychord){1, { {MODKEY|ControlMask|ShiftMask, XK_q}	},  quit,			{1} }),

	// DMENU AND DMSCRIPTS (Supper + p followed by KEY)
	&((Keychord){1, { {MODKEY, XK_d}						},	spawn,			{.v = dmenucmd } }),
	&((Keychord){2, { {MODKEY, XK_p}, {0, XK_b}				},	spawn,			SHCMD("dm-setbg") }),
	&((Keychord){2, { {MODKEY, XK_p}, {0, XK_x}				},	spawn,			SHCMD("dm-logout") }),
	&((Keychord){2, { {MODKEY, XK_p}, {0, XK_h}				},	spawn,			SHCMD("dm-hub") }),
	&((Keychord){2, { {MODKEY, XK_p}, {0, XK_m}				},	spawn,			SHCMD("dm-man") }),
	&((Keychord){2, { {MODKEY, XK_p}, {0, XK_o}				},	spawn,			SHCMD("dm-note") }),
	&((Keychord){2, { {MODKEY, XK_p}, {0, XK_r}				},	spawn,			SHCMD("dm-radio") }),
	&((Keychord){2, { {MODKEY, XK_p}, {0, XK_s}				},	spawn,			SHCMD("dm-record") }),
	&((Keychord){2, { {MODKEY, XK_p}, {0, XK_c}				},	spawn,			SHCMD("dm-confedit") }),
	&((Keychord){2, { {MODKEY, XK_p}, {0, XK_w}				},	spawn,			SHCMD("dm-websearch") }),
	&((Keychord){2, { {MODKEY, XK_p}, {0, XK_n}				},	spawn,			SHCMD("dm-wifi") }),

	// SYSTEM SETTINGS AND CONTROLS
	&((Keychord){1, { {0, XF86XK_MonBrightnessUp}			},	spawn,			SHCMD("brightnesscontrol.sh i") }),
	&((Keychord){1, { {0, XF86XK_MonBrightnessDown}			},	spawn,			SHCMD("brightnesscontrol.sh d") }),
	&((Keychord){1, { {0, XF86XK_AudioRaiseVolume}			},	spawn,			SHCMD("volumecontrol.sh -o i") }),
	&((Keychord){1, { {0, XF86XK_AudioLowerVolume}			},	spawn,			SHCMD("volumecontrol.sh -o d") }),
	&((Keychord){1, { {0, XF86XK_AudioMute}					},	spawn,			SHCMD("volumecontrol.sh -o m") }),
	// (Supper + i followed by KEY)
	&((Keychord){2, { {MODKEY, XK_i}, {0, XK_n}				},	spawn,			SHCMD("nm-connection-editor") }),
	&((Keychord){2, { {MODKEY, XK_i}, {0, XK_a}				},	spawn,			SHCMD("pavucontrol") }),
	&((Keychord){2, { {MODKEY, XK_i}, {0, XK_b}				},	spawn,			SHCMD("nitrogen") }),

	// CODE EDITOR (Supper + c followed by KEY)
	&((Keychord){2, { {MODKEY, XK_c}, {0, XK_c}				},	spawn,			SHCMD("code") }),
	&((Keychord){2, { {MODKEY, XK_c}, {0, XK_e}				},	spawn,			SHCMD("emacsclient -c -a 'emacs'") }),
	&((Keychord){2, { {MODKEY, XK_c}, {0, XK_a}				},	spawn,			SHCMD("emacsclient -c -a 'emacs' --eval '(emms)' --eval '(emms-play-directory-tree \"~/Music/\")'") }),
	&((Keychord){2, { {MODKEY, XK_c}, {0, XK_b}				},	spawn,			SHCMD("emacsclient -c -a 'emacs' --eval '(ibuffer)'") }),
	&((Keychord){2, { {MODKEY, XK_c}, {0, XK_d}				},	spawn,			SHCMD("emacsclient -c -a 'emacs' --eval '(dired nil)'") }),
	&((Keychord){2, { {MODKEY, XK_c}, {0, XK_m}				},	spawn,			SHCMD("mousepad") }),

	// TAG KEY
	&((Keychord){1, { {ALTKEY, XK_Tab}						},  view,			{0} }),
	&((Keychord){1, { {MODKEY, XK_comma}					},	tagmon,			{.i = -1 } }),
	&((Keychord){1, { {MODKEY, XK_period}					}, 	tagmon,			{.i = +1 } }),
	&((Keychord){1, { {MODKEY|ControlMask|ShiftMask,XK_0}	},	tag,			{.ui = ~0 } }),
	&((Keychord){1, { {MODKEY|ControlMask, XK_0}			},	view,			{.ui = ~0 } }),

	// SCREEN/MONITOR
	&((Keychord){1, { {MODKEY|ShiftMask, XK_Left}			},	focusmon,		{.i = -1 } }),
	&((Keychord){1, { {MODKEY|ShiftMask, XK_Right}			}, 	focusmon,		{.i = +1 } }),

	// CLIENT
	&((Keychord){1, { {MODKEY, XK_j}						},	focusstack,		{.i = +1 } }),
	&((Keychord){1, { {MODKEY, XK_k}						},	focusstack,		{.i = -1 } }),
	&((Keychord){1, { {MODKEY|ShiftMask, XK_j}				},	rotatestack,	{.i = +1 } }),
	&((Keychord){1, { {MODKEY|ShiftMask, XK_k}				},	rotatestack,	{.i = -1 } }),
	&((Keychord){1, { {MODKEY, XK_h}						},	setmfact,		{.f = -0.05} }),
	&((Keychord){1, { {MODKEY, XK_l}						},	setmfact,		{.f = +0.05} }),
	&((Keychord){1, { {MODKEY|ShiftMask, XK_h}				},	setcfact,		{.f = +0.25} }),
	&((Keychord){1, { {MODKEY|ShiftMask, XK_l}				},	setcfact,		{.f = -0.25} }),
	&((Keychord){1, { {MODKEY|ShiftMask, XK_o}				},	setcfact,		{.f =  0.00} }),
	&((Keychord){1, { {MODKEY, XK_m}						},	focusmaster,	{0} }),
	&((Keychord){1, { {MODKEY, XK_BackSpace}				},	zoom,			{0} }),
	&((Keychord){1, { {MODKEY, XK_q}						},	killclient,		{0} }),

	// LAYOUT
	&((Keychord){1, { {MODKEY|ShiftMask, XK_Up}				},	incnmaster,		{.i = +1 } }),
	&((Keychord){1, { {MODKEY|ShiftMask, XK_Down}			},	incnmaster,		{.i = -1 } }),
	&((Keychord){1, { {MODKEY, XK_Tab}						},	cyclelayout,	{.i = +1 } }),
	&((Keychord){1, { {MODKEY|ShiftMask, XK_Tab}			},	cyclelayout,	{.i = -1 } }),
	&((Keychord){2, { {MODKEY, XK_l}, {0, XK_1}				},	setlayout,		{.v = &layouts[0]} }),
	&((Keychord){2, { {MODKEY, XK_l}, {0, XK_2}				},	setlayout,		{.v = &layouts[1]} }),
	&((Keychord){2, { {MODKEY, XK_l}, {0, XK_3}				},	setlayout,		{.v = &layouts[2]} }),
	&((Keychord){2, { {MODKEY, XK_l}, {0, XK_4}				},	setlayout,		{.v = &layouts[3]} }),
	&((Keychord){2, { {MODKEY, XK_l}, {0, XK_5}				},	setlayout,		{.v = &layouts[4]} }),
	&((Keychord){2, { {MODKEY, XK_l}, {0, XK_6}				},	setlayout,		{.v = &layouts[5]} }),
	&((Keychord){2, { {MODKEY, XK_l}, {0, XK_7}				},	setlayout,		{.v = &layouts[6]} }),
	&((Keychord){2, { {MODKEY, XK_l}, {0, XK_8}				},	setlayout,		{.v = &layouts[7]} }),
	&((Keychord){2, { {MODKEY, XK_l}, {0, XK_9}				},	setlayout,		{.v = &layouts[8]} }),
	&((Keychord){2, { {MODKEY, XK_l}, {0, XK_0}				},	setlayout,		{.v = &layouts[9]} }),
	&((Keychord){1, { {MODKEY|ShiftMask, XK_space}			},	togglefloating,	{0} }),
	&((Keychord){1, { {MODKEY, XK_space}					},	setlayout,		{0} }),

	// GAPS
	// (Super + g followed by KEY) On the fly gaps change
	&((Keychord){2, { {MODKEY, XK_g}, {0, XK_t}				},  togglegaps,		{0} }),
	&((Keychord){2, { {MODKEY, XK_g}, {0, XK_d}				},  defaultgaps,	{0} }),
	// lol of course i don't use this overwhelming gaps settings.
	&((Keychord){2, { {MODKEY, XK_g}, {0, XK_k}				},	incrgaps,		{.i = +4 } }),
	&((Keychord){2, { {MODKEY, XK_g}, {0, XK_j}				},	incrgaps,		{.i = -4 } }),
	&((Keychord){3, { {MODKEY, XK_g}, {0, XK_o}, {0, XK_k}	},	incrogaps,		{.i = +4 } }),
	&((Keychord){3, { {MODKEY, XK_g}, {0, XK_o}, {0, XK_j}	},	incrogaps,		{.i = -4 } }),
	&((Keychord){3, { {MODKEY, XK_g}, {0, XK_i}, {0, XK_k}	},	incrigaps,		{.i = +4 } }),
	&((Keychord){3, { {MODKEY, XK_g}, {0, XK_i}, {0, XK_j}	},	incrigaps,		{.i = -4 } }),
	&((Keychord){3, { {MODKEY, XK_g}, {0, XK_h}, {0, XK_k}	},	incrihgaps,		{.i = +4 } }),
	&((Keychord){3, { {MODKEY, XK_g}, {0, XK_h}, {0, XK_j}	},	incrihgaps,		{.i = -4 } }),
	&((Keychord){3, { {MODKEY, XK_g}, {0, XK_v}, {0, XK_k}	},	incrivgaps,		{.i = +4 } }),
	&((Keychord){3, { {MODKEY, XK_g}, {0, XK_v}, {0, XK_j}	},	incrivgaps,		{.i = -4 } }),
	&((Keychord){1, { {MODKEY|ALTKEY, XK_y}					},	incrohgaps,		{.i = +4 } }),
	&((Keychord){1, { {MODKEY|ALTKEY, XK_o}					},	incrohgaps,		{.i = -4 } }),
	&((Keychord){1, { {MODKEY|ShiftMask, XK_y}				},	incrovgaps,		{.i = +4 } }),
	&((Keychord){1, { {MODKEY|ShiftMask, XK_o}				},	incrovgaps,		{.i = -4 } }),

	// ANOTHER TOGGLE
	&((Keychord){1, { {MODKEY, XK_f}						},	togglefullscr,	{0} }),
	&((Keychord){2, { {MODKEY, XK_grave}, {0, XK_1}			},	togglescratch,	{.ui = 0 } }),
	&((Keychord){2, { {MODKEY, XK_grave}, {0, XK_2}			},	togglescratch,	{.ui = 1 } }),
	&((Keychord){2, { {MODKEY, XK_grave}, {0, XK_3}			},	togglescratch,	{.ui = 2 } }),
	&((Keychord){1, { {MODKEY|ShiftMask, XK_F5}				},	xrdb,			{.v = NULL } }),

	// VIEW TAG
	TAGKEYS( XK_1, 0 )
	TAGKEYS( XK_2, 1 )
	TAGKEYS( XK_3, 2 )
	TAGKEYS( XK_4, 3 )
	TAGKEYS( XK_5, 4 )
	TAGKEYS( XK_6, 5 )
	TAGKEYS( XK_7, 6 )
	TAGKEYS( XK_8, 7 )
	TAGKEYS( XK_9, 8 )
	TAGKEYS( XK_0, 9 )
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	/* placemouse options, choose which feels more natural:
	 *    0 - tiled position is relative to mouse cursor
	 *    1 - tiled postiion is relative to window center
	 *    2 - mouse pointer warps to window center
	 *
	 * The moveorplace uses movemouse or placemouse depending on the floating state
	 * of the selected client. Set up individual keybindings for the two if you want
	 * to control these separately (i.e. to retain the feature to move a tiled window
	 * into a floating position).
	 */
	{ ClkClientWin,         MODKEY,         Button1,        moveorplace,    {.i = 1} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};


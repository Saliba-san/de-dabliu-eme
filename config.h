/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 0;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char font[]            = "Times New Roman, 11";
static const char dmenufont[]       = "Times New Roman-11:antialias=true:autohint=true";
static const char col_normBg[]       = "#282828";
static const char col_selBorder[]    = "#d79921";
static const char col_normFg[]       = "#928374";
static const char col_selFg[]        = "#d79921";
static const char col_normBorder[]   = "#282828"; 
static const char col_selBg[]        = "#282828"; 
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_normFg, col_normBg, col_normBorder },
	[SchemeSel]  = { col_selFg, col_selBg,  col_selBorder },
};

/* tagging */
static const char *tags[] = { "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "XI"};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.6; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

/* constant definition section */
  /* volume */
  static const char *brupcmd[] = { "xbacklight", "-inc", "10", NULL };
  static const char *brdowncmd[] = { "xbacklight", "-dec", "10", NULL };
  static const char *calcursecmd[]  = { "st", "-e", "calcurse", NULL };
  static const char *mocpcmd[]  = { "st", "-e", "mocp", NULL };

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "ll=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "lMl",      monocle },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz},
	{ "HHH",      grid},
 	{ "[@]",      spiral },
 	{ "[\\]",     dwindle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_normBg, "-nf", col_normFg, "-sb", col_normBorder, "-sf", col_selFg, NULL };
static const char *termcmd[]  = { "st", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_n,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_o,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_b,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY|ShiftMask,             XK_b,      setlayout,      {.v = &layouts[6]} },
	{ MODKEY,												XK_g,      setlayout,      {.v = &layouts[7]} },
	{ MODKEY,                       XK_r,      setlayout,      {.v = &layouts[8]} },
	{ MODKEY|ShiftMask,             XK_r,      setlayout,      {.v = &layouts[9]} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
  //customizado
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
  //my definitions
  //volume
  { 0,               XF86XK_AudioLowerVolume, spawn,  SHCMD("pactl set-sink-volume 0 -5%; kill -44 $(pidof dwmblocks)") },
  { 0,               XF86XK_AudioMute,        spawn,  SHCMD("pactl set-sink-mute 0 toggle; kill -44 $(pidof dwmblocks)") },
  { 0,               XF86XK_AudioRaiseVolume, spawn,  SHCMD("pactl set-sink-volume 0 +5%; kill -44 $(pidof dwmblocks)") },
	//music
  { MODKEY,          XF86XK_AudioLowerVolume, spawn,		SHCMD("mocp --previous; kill -45 $(pidof dwmblocks) ") },
  { MODKEY,          XF86XK_AudioMute,        spawn,		SHCMD("mocp --toggle-pause; kill -45 $(pidof dwmblocks)") },
  { MODKEY,          XF86XK_AudioRaiseVolume, spawn,		SHCMD("mocp --next; kill -45 $(pidof dwmblocks)") },
  { MODKEY|ShiftMask,XF86XK_AudioMute,        spawn,		SHCMD("mocp -S; mocp -p; kill -45 $(pidof dwmblocks)") },
  //brightness
  { 0,               XF86XK_MonBrightnessUp,  spawn,     {.v = brupcmd } },
  { 0,               XF86XK_MonBrightnessDown,spawn,     {.v = brdowncmd} },
  //print
  { 0,               XK_Print,                 spawn,    SHCMD("maim | xclip -selection clipboard -t image/png")},
  { 0|ShiftMask,     XK_Print,                 spawn,    SHCMD("maim -s | xclip -selection clipboard -t image/png")},
  { MODKEY,          XK_Print,                 spawn,    SHCMD("maim ~/Pictures/Screenshot/$(date +%d-%m-%y-%H:%M:%S).png")},
  { MODKEY|ShiftMask,XK_Print,                 spawn,    SHCMD("maim -s ~/Pictures/Screenshot/$(date +%d-%m-%y-%H:%M:%S).png")},
  //lauch
  { MODKEY|ShiftMask,XK_c,                     spawn,    {.v = calcursecmd } },
  { MODKEY|ShiftMask,XK_m,                     spawn,    {.v = mocpcmd } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};


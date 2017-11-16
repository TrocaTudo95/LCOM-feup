static char *snakeHeadUp[] = {
/* columns rows colors */
"23 30 9",
"  0", //black
"X 2", //00A800
"# 7", //gray66
"> 16", //005400
"5 23", //A8FFA8
"g 42", //54A854
"z 47", //FFA8FF
"B 56", //gray33
"F 63", //gray100
/* pixels */
"        >>>>>>>        ",
"       >XXXXXXX>       ",
"      XXXXXXXXXXX      ",
"     XXXX>>X>>XXXX     ",
"    XXXXX>XXX>XXXXX    ",
"   >XXXXXXXXXXXXXXX>   ",
"   XXXXXXXXXXXXXXXXX   ",
"  >XXXXXXXXXXXXXXXXX>  ",
"  XXXXXXXXXXXXXXXXXXX  ",
" >XXXXXXXXXXXXXXXXXXX> ",
" XXXgggXXXXXXXXXgggXX> ",
" X5#BB#5XXXXXXX5#BB#5X ",
">5z    #FXXXXXF#   >F5>",
"#F> >> >F5XXXFF  >> BFg",
"#F> >>> FFXXgF# >>> >F#",
"#FB >> >FFg>gFF  >> BF#",
"#F#    #FFg>#FF#   >FF#",
"#FF5##5FFFg>gFFF###FFF#",
"#FFFFFFFFFXXXFFFFFFFFF#",
"BFFFFFFFFgXXXgFFFFFFFFB",
">gFFFFFFgXXXXX#FFFFFFg ",
" XXg55#gXXXXXXXg#55gXX ",
" >XXXXXXXXXXXXXXXXXXX> ",
"  XXXXXXXXXXXXXXXXXXX  ",
"  >XXXXXXXXXXXXXXXXX>  ",
"   XXXXXXXXXXXXXXXXX   ",
"   >XXXXXXXXXXXXXXX>   ",
"    XXXXXXXXXXXXXXX    ",
"    XXXXXXXXXXXXXXX    ",
"    >XXXXXXXXXXXXXX    "
};

/* XPM */
static char *body[] = {
/* columns rows colors chars-per-pixel */
		"15 15 1",
		"X 2", //00A800
/* pixels */
"XXXXXXXXXXXXXXX",
"XXXXXXXXXXXXXXX",
"XXXXXXXXXXXXXXX",
"XXXXXXXXXXXXXXX",
"XXXXXXXXXXXXXXX",
"XXXXXXXXXXXXXXX",
"XXXXXXXXXXXXXXX",
"XXXXXXXXXXXXXXX",
"XXXXXXXXXXXXXXX",
"XXXXXXXXXXXXXXX",
"XXXXXXXXXXXXXXX",
"XXXXXXXXXXXXXXX",
"XXXXXXXXXXXXXXX",
"XXXXXXXXXXXXXXX",
"XXXXXXXXXXXXXXX"
};


static char *snakeHeadLeft[] = {
/* columns rows colors */
"30 23 9",
"  0", //black
"X 2", //00A800
"# 7", //gray66
"> 16", //005400
"5 23", //A8FFA8
"g 42", //54A854
"z 47", //FFA8FF
"B 56", //gray33
"F 63", //gray100
/* pixels */
"            >g#####B          ",
"         >>X5FFFFFFFgX>       ",
"       >XXX5FB>BFFFFFXXX>     ",
"     >XXXXX#>   >FFFFgXXXX>   ",
"    XXXXXXgB >>> #FFF5XXXXXXXX",
"   XXXXXXXgB >>> #FFF5XXXXXXXX",
"  XXXXXXXXg#  >  #FFF#XXXXXXXX",
" >XXXXXXXXX5#   #FFFFgXXXXXXXX",
">XXXXXXXXXXXFF#FFFFF#XXXXXXXXX",
">XX>>XXXXXXXXFFFFFFgXXXXXXXXXX",
">XX>XXXXXXXXXXgg#gXXXXXXXXXXXX",
">XXXXXXXXXXXXXX>>>XXXXXXXXXXXX",
">XX>XXXXXXXXXXXgggXXXXXXXXXXXX",
">XX>>XXXXXXXX5FFFFFgXXXXXXXXXX",
">XXXXXXXXXXXFFFFFFFFgXXXXXXXXX",
" >XXXXXXXXX5#> >#FFFFgXXXXXXXX",
"  XXXXXXXXg#  >  5FFF#XXXXXXXX",
"   XXXXXXXgB >>> #FFF5XXXXXXXX",
"    XXXXXXgB >>> #FFF5XXXXXXX>",
"     >XXXXX#     5FFFgXXXX>   ",
"       >XXX5z>>B#FFFFXXX>     ",
"         >XX5FFFFFFFgX>       ",
"            >######B>         "
};

static char *snakeHeadRight[] = {
/* columns rows colors chars-per-pixel */
"30 23 9",
"  0", //black
"X 2", //00A800
"# 7", //gray66
"> 16", //005400
"5 23", //A8FFA8
"g 42", //54A854
"z 47", //FFA8FF
"B 56", //gray33
"F 63", //gray100
/* pixels */
"         >B######>            ",
"       >XgFFFFFFF5XX>         ",
"     >XXXFFFF#B>>z5XXX>       ",
"   >XXXXgFFF5     #XXXXX>     ",
">XXXXXXX5FFF# >>> BgXXXXXX    ",
"XXXXXXXX5FFF# >>> BgXXXXXXX   ",
"XXXXXXXX#FFF5  >  #gXXXXXXXX  ",
"XXXXXXXXgFFFF#> >#5XXXXXXXXX> ",
"XXXXXXXXXgFFFFFFFFXXXXXXXXXXX>",
"XXXXXXXXXXgFFFFF5XXXXXXXX>>XX>",
"XXXXXXXXXXXXgggXXXXXXXXXXX>XX>",
"XXXXXXXXXXXX>>>XXXXXXXXXXXXXX>",
"XXXXXXXXXXXXg#ggXXXXXXXXXX>XX>",
"XXXXXXXXXXgFFFFFFXXXXXXXX>>XX>",
"XXXXXXXXX#FFFFF#FFXXXXXXXXXXX>",
"XXXXXXXXgFFFF#   #5XXXXXXXXX> ",
"XXXXXXXX#FFF#  >  #gXXXXXXXX  ",
"XXXXXXXX5FFF# >>> BgXXXXXXX   ",
"XXXXXXXX5FFF# >>> BgXXXXXX    ",
"   >XXXXgFFFF>   >#XXXXX>     ",
"     >XXXFFFFFB>BF5XXX>       ",
"       >XgFFFFFFF5X>>         ",
"          B#####g>            "
};


static char *snakeHeadDown[] = {
/* columns rows colors chars-per-pixel */
"23 30 9",
"  0", //black //USED
"X 2", //00A800 //USED
"# 7", //gray66 //USED
"> 16", //005400 //USED
"5 23", //A8FFA8 //USED
"g 42", //54A854 //USED
"z 47", //FFA8FF //USED
"B 56", //gray33 //USED
"F 63", //gray100 //USED
/* pixels */
"    XXXXXXXXXXXXXX>    ",
"    XXXXXXXXXXXXXXX    ",
"    XXXXXXXXXXXXXXX    ",
"   >XXXXXXXXXXXXXXX>   ",
"   XXXXXXXXXXXXXXXXX   ",
"  >XXXXXXXXXXXXXXXXX>  ",
"  XXXXXXXXXXXXXXXXXXX  ",
" >XXXXXXXXXXXXXXXXXXX> ",
" XXg55#gXXXXXXXg#55gXX ",
" gFFFFFF#XXXXXgFFFFFFg>",
"BFFFFFFFFgXXXgFFFFFFFFB",
"#FFFFFFFFFXXXFFFFFFFFF#",
"#FFF###FFFg>gFFF5##5FF#",
"#FF>   #FF#>gFF#    #F#",
"#FB >>  FFg>gFF> >> BF#",
"#F> >>> #FgXXFF >>> >F#",
"gFB >>  FFXXX5F> >> >F#",
">5F>   #FXXXXXF#    z5>",
" X5#BB#5XXXXXXX5#BB#5X ",
" >XXgggXXXXXXXXXgggXXX ",
" >XXXXXXXXXXXXXXXXXXX> ",
"  XXXXXXXXXXXXXXXXXXX  ",
"  >XXXXXXXXXXXXXXXXX>  ",
"   XXXXXXXXXXXXXXXXX   ",
"   >XXXXXXXXXXXXXXX>   ",
"    XXXXX>XXX>XXXXX    ",
"     XXXX>>X>>XXXX     ",
"      XXXXXXXXXXX      ",
"       >XXXXXXX>       ",
"        >>>>>>>        "
};



static char *apple[] = {
/* columns rows colors */
"48 49 14",
"  0", //gray100 //USED
"+ 39", //FFAAAA //USED //possible conflict with : (red)
"@ 46", //FFAA55 //USED
"& 60", //FF5555 //USED
"* 52", //FF5500 //USED
": 4", //red //USED //good new color?
"9 20", //AA5500 //USED
"e 5", //AA0000 //USED //conflict with :
"a 10", //55AA55 //USED
"g 56", //555555 //USED
"h 48", //555500 //USED
"z 32", //550000 //USED
"Z 16", //005500 //USED
"F 1", //black //USED //FIX
/* pixels */
"                          hhha                  ",
"                         hh9hFF                 ",
"                         F99@@9F                ",
"                         F999*@hh               ",
"                         z9999zh                ",
"                        gz9999F                 ",
"           hhFFFFFFF    hz999F                  ",
"         hFFFFFFFFFFFh  hz99zh                  ",
"        FFFFzeeeeeezFFF hz99F                   ",
"       FFFeeee&&&:eeeFFhhh9hF                   ",
"      FFFeee&+++++eeeeFFF99zh                   ",
"     FFzee&++++++&eeeeezF99Fhhhhhhha            ",
"    FFzee&++++++&eeeeeeee99FFFFFFFFFFFh         ",
"   FFFee&+++++&eeeeeeeeee99ezzzzzzzFFFFFF       ",
"  hFFee&++++&eeeeeeeeeeee99eeeeeeeeeezFFFFF     ",
"  FFze&++++&eeeeeeeeeeeee9eeeeeeeeeeeeeezFFF    ",
" hFFee+++++eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeFFF   ",
" FFzee++++&eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeFFF  ",
" FFeee+++:eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeFFh ",
"hFzeee:&eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeezFF ",
"FFeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeFF ",
"FFeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeezFh",
"FFeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeezFF",
"FzeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeFF",
"FzeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeFF",
"FFeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeFF",
"FFeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeFF",
"FFeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeFF",
"ZFeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeFF",
"hFzeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeFF",
" FFeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeezFh",
" FFeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeezF ",
" hFzeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeFF ",
"  FFeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeezFF ",
"  FFeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeFF  ",
"  hFzeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeezFF  ",
"   FFeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeFFh  ",
"   ZFzeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeezFF   ",
"    FFeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeFF    ",
"    ZFFeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeezFF    ",
"     FFzeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeezFF     ",
"      FFFeeeeeeeeeeeeeeeeeeeeeeeeeeeeeezFF      ",
"       FFFzeeeeeeeeeeeeeeeeeeeeeeeeeeezFF       ",
"        FFFFzeeeeeeeeeeeeeeeeeeeeeeeeFFF        ",
"          FFFFzeeeeeeeeeeeeeeeeeeeezFFF         ",
"            ZFFFFzeeeeeeeeeeeeeeezFFFh          ",
"              hFFFFzzzeeeeeeeeezFFFF            ",
"                 FFFFFFFFFFFFFFFFF              ",
"                    hFFFFFFFFFZ                 "
};

static char *scoreXpm[] = {
/* columns rows colors */
"72 20 2",
"  0",
"F 63",
/* pixels */
"    FFFFFFF                                                             ",
"  FFFFFFFFFFF                                                           ",
" FFF      FFFF                                                          ",
"FFF         FF                                                          ",
"FF           FF                                                         ",
"FF           FF      FFFFF        FFFFF       FF FFFF     FFFFF       FF",
"FFF                FFFFFFFF     FFFFFFFFF     FFFFFFF   FFFFFFFFF     FF",
" FFF              FFF    FFF   FFF     FFF    FFF      FFF     FFF      ",
" FFFFFFFF         FF      FFF  FF       FF    FFF      FF       FF      ",
"  FFFFFFFFFF     FF        FF FFF       FFF   FF      FF         FF     ",
"      FFFFFFFF   FF           FF         FF   FF      FF         FF     ",
"          FFFF   FF           FF         FF   FF      FFFFFFFFFFFFF     ",
"            FFF  FF           FF         FF   FF      FFFFFFFFFFFFF     ",
"F            FF  FF           FF         FF   FF      FF                ",
"F            FF  FF        FF FF         FF   FF      FF                ",
"FF           FF  FF        FF FFF       FFF   FF      FFF               ",
"FFF         FF    FF      FF   FF       FF    FF       FF        FF     ",
" FFF       FFF    FFF    FFF   FFF     FFF    FF       FFFF    FFF      ",
"  FFFFFFFFFFF      FFFFFFFF     FFFFFFFFF     FF        FFFFFFFFF     FF",
"    FFFFFFF          FFFFF        FFFFF       FF          FFFFFF      FF"
};

static char *numZero[] = {
/* columns rows colors */
"20 20 2",
"  0",
"F 63",
/* pixels */
"       FFFFFF       ",
"      FFFFFFFFF     ",
"     FFFFFFFFFFF    ",
"    FFFF     FFFF   ",
"   FFFF       FFF   ",
"   FFF        FFFF  ",
"  FFFF         FFF  ",
"  FFF          FFF  ",
"  FFF          FFF  ",
"  FFF          FFF  ",
"  FFF          FFF  ",
"  FFF          FFF  ",
"  FFF          FFF  ",
"  FFFF        FFFF  ",
"   FFF        FFF   ",
"   FFFF      FFFF   ",
"    FFFF    FFFF    ",
"    FFFFFFFFFFFF    ",
"     FFFFFFFFFF     ",
"       FFFFFF       "
};

static char *numOne[] = {
/* columns rows colors */
"20 20 2",
"  0",
"F 63",
/* pixels */
"         FFF        ",
"       FFFFF        ",
"     FFFFFFF        ",
"    FFFFFFFF        ",
"    FFF FFFF        ",
"        FFFF        ",
"        FFFF        ",
"        FFFF        ",
"        FFFF        ",
"        FFFF        ",
"        FFFF        ",
"        FFFF        ",
"        FFFF        ",
"        FFFF        ",
"        FFFF        ",
"        FFFF        ",
"        FFFF        ",
"    FFFFFFFFFFFF    ",
"    FFFFFFFFFFFF    ",
"    FFFFFFFFFFFF    "
};

/* XPM */
static char *numTwo[] = {
/* columns rows colors */
"20 20 2",
"  0",
"F 63",
/* pixels */
"        FFFFFF      ",
"     FFFFFFFFFFF    ",
"    FFFFFFFFFFFFF   ",
"   FFFFF    FFFFFF  ",
"  FFFF        FFFF  ",
"  FFF         FFFF  ",
"              FFFF  ",
"             FFFF   ",
"            FFFF    ",
"          FFFFF     ",
"        FFFFF       ",
"      FFFFF         ",
"     FFFF           ",
"   FFFFF            ",
"   FFFF             ",
"  FFFF              ",
"  FFFF              ",
"  FFFFFFFFFFFFFFF   ",
"  FFFFFFFFFFFFFFFF  ",
"   FFFFFFFFFFFFFFF  "
};

/* XPM */
static char *numThree[] = {
/* columns rows colors */
"20 20 2",
"  0",
"F 63",
/* pixels */
"       FFFFFF       ",
"     FFFFFFFFFF     ",
"   FFFFFFFFFFFFF    ",
"   FFFF     FFFFF   ",
"   FF        FFFF   ",
"             FFFF   ",
"            FFFFF   ",
"          FFFFFF    ",
"     FFFFFFFFFF     ",
"     FFFFFFFF       ",
"          FFFFFF    ",
"            FFFFF   ",
"             FFFFF  ",
"              FFFF  ",
"  FFF         FFFF  ",
"  FFF        FFFFF  ",
"  FFFFF     FFFFF   ",
"   FFFFFFFFFFFFFF   ",
"    FFFFFFFFFFF     ",
"      FFFFFFF       "
};

/* XPM */
static char *numFour[] = {
/* columns rows colors */
"20 20 2",
"  0",
"F 63",
/* pixels */
"           FFFF     ",
"          FFFFF     ",
"          FFFFF     ",
"         FFFFFF     ",
"        FFF FFF     ",
"       FFF  FFF     ",
"      FFFF  FFF     ",
"     FFFF   FFF     ",
"     FFF    FFF     ",
"    FFF     FFF     ",
"   FFF      FFF     ",
"  FFFFFFFFFFFFFFFF  ",
" FFFFFFFFFFFFFFFFFF ",
" FFFFFFFFFFFFFFFFFF ",
"            FFF     ",
"            FFF     ",
"            FFF     ",
"            FFF     ",
"            FFF     ",
"            FFF     "
};

/* XPM */
static char *numFive[] = {
/* columns rows colors */
"20 20 2",
"  0",
"F 63",
/* pixels */
"  FFFFFFFFFFFFFFF   ",
"  FFFFFFFFFFFFFFF   ",
"   FFFFFFFFFFFFF    ",
"   FFF              ",
"   FFF              ",
"   FFF FFFFFFF      ",
"  FFFFFFFFFFFFFF    ",
"  FFFFFFFFFFFFFFF   ",
"  FFFFFF     FFFF   ",
"  FFFF        FFFF  ",
"  FFF          FFF  ",
"               FFF  ",
"               FFF  ",
"               FFF  ",
"              FFFF  ",
"  FFF        FFFF   ",
"  FFFF      FFFFF   ",
"   FFFFFFFFFFFFF    ",
"    FFFFFFFFFF      ",
"      FFFFFF        "
};

/* XPM */
static char *numSix[] = {
/* columns rows colors */
"20 20 2",
"  0",
"F 63",
/* pixels */
"          FFF       ",
"         FFFF       ",
"        FFFF        ",
"      FFFFF         ",
"     FFFFF          ",
"    FFFF            ",
"    FFF             ",
"   FFFF             ",
"   FFF  FFFFFF      ",
"  FFFFFFFFFFFFFF    ",
"  FFFFFFFFFFFFFFF   ",
"  FFFF       FFFFF  ",
"  FFF          FFF  ",
"  FFF          FFF  ",
"  FFF          FFF  ",
"  FFFF        FFFF  ",
"   FFFF      FFFF   ",
"    FFFFFFFFFFFFF   ",
"     FFFFFFFFFF     ",
"      FFFFFFF       "
};

/* XPM */
static char *numSeven[] = {
/* columns rows colors */
"20 20 2",
"  0",
"F 63",
/* pixels */
"  FFFFFFFFFFFFFFFF  ",
"  FFFFFFFFFFFFFFFF  ",
"   FFFFFFFFFFFFFF   ",
"             FFFF   ",
"            FFFF    ",
"           FFFF     ",
"           FFF      ",
"          FFF       ",
"          FFF       ",
"         FFF        ",
"         FFF        ",
"        FFF         ",
"        FFF         ",
"       FFF          ",
"       FFF          ",
"       FF           ",
"      FFF           ",
"      FFF           ",
"     FFF            ",
"     FF             "
};

/* XPM */
static char *numEight[] = {
/* columns rows colors */
"20 20 2",
"  0",
"F 63",
/* pixels */
"      FFFFFFFF      ",
"    FFFFFFFFFFFF    ",
"   FFFFFFFFFFFFF    ",
"  FFFF       FFFF   ",
"  FFF         FFF   ",
"  FFF         FFF   ",
"  FFFFF     FFFFF   ",
"   FFFFFFFFFFFFF    ",
"    FFFFFFFFFF      ",
"    FFFFFFFFFFFF    ",
"   FFFFF    FFFFF   ",
"   FFF        FFFF  ",
"  FFF          FFF  ",
"  FFF          FFF  ",
"  FFF          FFF  ",
"  FFF          FFF  ",
"  FFFFF      FFFFF  ",
"   FFFFFFFFFFFFFF   ",
"    FFFFFFFFFFFF    ",
"      FFFFFFFF      "
};

/* XPM */
static char *numNine[] = {
/* columns rows colors */
"20 20 2",
"  0",
"F 63",
/* pixels */
"       FFFFFF       ",
"     FFFFFFFFFF     ",
"    FFF     FFFF    ",
"   FFF       FFFF   ",
"  FFF         FFFF  ",
"  FFF          FFF  ",
"  FFF          FFF  ",
"  FFF          FFF  ",
"  FFF          FFF  ",
"   FFF        FFFF  ",
"   FFFF     FFFFF   ",
"    FFFFFFFFFFFFF   ",
"      FFFFFFF FFF   ",
"             FFF    ",
"            FFF     ",
"           FFF      ",
"         FFFF       ",
"      FFFFFF        ",
"   FFFFFFF          ",
"   FFFF             "
};

/* XPM */
static char *twoPoints[] = {
/* columns rows colors */
"20 20 2",
"  0",
"F 63",
/* pixels */
"                    ",
"                    ",
"                    ",
"                    ",
"         FFF        ",
"         FFF        ",
"         FFF        ",
"                    ",
"                    ",
"                    ",
"                    ",
"                    ",
"                    ",
"         FFF        ",
"         FFF        ",
"         FFF        ",
"                    ",
"                    ",
"                    ",
"                    "
};

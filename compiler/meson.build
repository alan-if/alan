project('Alan compiler', 'c',
  version : '3.0beta8',
  default_options : ['warning_level=3'])

interpreter_include = include_directories('../interpreter')
iconv = dependency('iconv')

unittested = [
  'add',
  'adv',
  'atr',
  'cla',
  'description',
  'elm',
  'exp',
  'ext',
  'ifid',
  'ins',
  'lst',
  'prop',
  'res',
  'resource',
  'stm',
  'stx',
  'sym',
  'util',
  'vrb',
  'whr',
  'wrd'
]

unittests = []
foreach f : unittested
  unittests += [f+'Test']
endforeach

tm_c = [
  'pmParse',
  'pmPaSema',
  'pmErr',
  'smScan',
  'smScSema',
  'lmList'
]

others = [
  'alan',
  'alt',
  'article',
  'charset',
  'chk',
  'cnt',
  'context',
  'converter',
  'dump',
  'encode',
  'emit',
  'evt',
  'id',
  'initialize',
  'lim',
  'lmlog',
  'msg',
  'nam',
  'opt',
  'options',
  'rul',
  'sco',
  'scr',
  'set',
  'spa',
  'srcp',
  'stp',
  'str',
  'syn',
  'sysdep',
  'timing',
  'type',
  'wht'
]

alan_srcs = []
foreach f : tm_c + unittested + others
  alan_srcs += [ f + '.c' ]
endforeach
              
executable('alan',
           alan_srcs + [ 'alan.version.c' ],
           include_directories : interpreter_include,
           dependencies : iconv)


from subprocess import Popen, PIPE
from xml.dom import minidom
from os.path import split

__author__ = 'Thomas'

def is_location(instance, classes):
    try:
        parent_name = instance.attributes['PARENT'].value
        while parent_name != 'location':
            parent_class = [c for c in classes
                            if c.attributes['NAME'].value == parent_name
                           ]
            parent_name = parent_class[0].attributes['PARENT'].value
        return True
    except Exception as e:
        return False

def get_locations(xmltree, ignore):
    classes = xmltree.getElementsByTagName("class")
    return [i for i in xmltree.getElementsByTagName("instance")
            if is_location(i, classes) and not (i.attributes['NAME'].value.lower() in map(str.lower, ignore))
           ]

def compile_game_to_xml(filename):
    process = Popen(["alan", "-xml", filename], stdout=PIPE)
    output = process.communicate(None)[0]
    return minidom.parse(split(filename)[1]+".xml")


def get_exits(location, ignore):
    return [e for e in location.getElementsByTagName('exit')
            if not (e.attributes['TARGET'].value.lower() in map(str.lower, ignore))
            ]


def dot_for_location_header(l):
    name = l.attributes['NAME'].value.lower()
    return '{0} [label="{0}"];'.format(name)


def dot_for_exit(location_name, x):
    target = x.attributes['TARGET'].value.lower()
    direction = x.attributes['DIRECTION'].value.lower()
    return "  {0} -> {1} [label={2}];".format(location_name.lower(), target, direction)

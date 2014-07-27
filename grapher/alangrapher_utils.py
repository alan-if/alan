from subprocess import Popen, PIPE
from xml.dom import minidom

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

def get_locations(xmltree):
    classes = xmltree.getElementsByTagName("class")
    return [i for i in xmltree.getElementsByTagName("instance")
            if is_location(i, classes)
           ]

def compile_game_to_xml(gamename):
    # process = Popen(["alan", "-xml", gamename], stdout=PIPE)
    # output = process.communicate(None)[0]
    return minidom.parse(gamename+".xml")


def get_exits(location):
    return location.getElementsByTagName('exit')


def dot_for_location_header(l):
    name = l.attributes['NAME'].value
    return '{0} [label="{0}"];'.format(name)


def dot_for_exit(location_name, x):
    target = x.attributes['TARGET'].value
    direction = x.attributes['DIRECTION'].value
    return "  {0} -> {1} [label={2}];".format(location_name, target, direction)
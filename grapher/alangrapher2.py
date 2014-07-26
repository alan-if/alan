#!/usr/bin/env python3
from subprocess import Popen, PIPE
from xml.dom import minidom

import unittest

def compile_game_to_xml(gamename):
    process = Popen(["alan", "-xml", gamename], stdout=PIPE)
    output = process.communicate(None)[0]
    return minidom.parse(gamename+".xml")


class AlanGrapherTests(unittest.TestCase):
    def test_empty_xml_returns_no_instances_and_no_classes(self):
        document = """<adventure>
                        <classes>
                        </classes>
                        <instances>
                        </instances>
                    </adventure>"""

        xmltree = minidom.parseString(document)
        self.assertEqual(0, len(xmltree.getElementsByTagName("instance")))
        self.assertEqual(0, len(xmltree.getElementsByTagName("class")))

    def test_finds_instance(self):
        document = """<adventure>
                        <classes>
                        </classes>
                        <instances>
                            <instance NAME='loc1'></instance>
                        </instances>
                    </adventure>"""

        xmltree = minidom.parseString(document)
        instances = xmltree.getElementsByTagName("instance")
        self.assertEqual(1, len(instances))
        self.assertEqual("loc1", instances[0].attributes['NAME'].value)

class LocationFinderTests(unittest.TestCase):
    def test_fail(self):
        self.fail("NYI")

if __name__ == '__main__':
    unittest.main()
# else:
#     xmltree = compile_game_to_xml("adventV3")
#     instance_list = xmltree.getElementsByTagName('instance')
#     print(len(instance_list), "instances found")
#     for s in instance_list :
#         print(s.attributes['NAME'].value)
#
#     class_list = xmltree.getElementsByTagName('class')

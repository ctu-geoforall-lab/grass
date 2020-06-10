#!/usr/bin/env python3
from grass.gunittest.case import TestCase
from grass.gunittest.main import test
from grass.gunittest.gmodules import call_module
import grass.script as gscript

output = 'test05'
buffer = 'basins'
distance1=100
distance2=200
ref_stats = '1 1554947\n2 58014\n* 412039'

class Test_r_buffer(TestCase):
    @classmethod
    def setUpClass(cls):
        cls.use_temp_region()


    @classmethod
    def tearDownClass(cls):
        cls.del_temp_region()

#create buffer with distance 100 metres
    def test_check_distance(self):
        self.assertModule('r.buffer', output=output, input=buffer, overwrite=True, distances=distance1)

        stats = call_module('r.stats', input=output, flags='c').rstrip('\n')
        self.assertEqual(stats, ref_stats)

if __name__ == '__main__':
    test()

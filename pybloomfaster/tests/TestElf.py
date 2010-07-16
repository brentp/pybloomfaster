import unittest
from pybloomfaster import Elf

class TestInitialize(unittest.TestCase):
    def setUp(self):
        pass

    def test_init(self):
        e = Elf(200)
        self.assert_(isinstance(e, Elf))

class TestContains(unittest.TestCase):
    def setUp(self):
        self.e = Elf(100)
        for i in range(100):
            self.e.add(str(i))
    def testContains(self):
        for i in range(100):
            self.assert_(str(i) in self.e)

if __name__ == "__main__":
    unittest.main()

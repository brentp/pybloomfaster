import unittest
from bloomfaster import Elf
import os

class TestInitialize(unittest.TestCase):
    def setUp(self):
        pass

    def test_init(self):
        e = Elf(200)
        self.assertTrue(isinstance(e, Elf))

class TestContains(unittest.TestCase):
    def setUp(self):
        self.e = Elf(100)
        for i in range(100):
            self.e.add(str(i))

    def testContains(self):
        for i in range(100):
            self.assertTrue(str(i) in self.e)

class TestSerialize(TestContains):
    def setUp(self):
        self.e = Elf(100)
        for i in range(100):
            self.e.add(str(i))
        self.e.save("_t.elf")

    def testSave(self):
        self.assertTrue(os.path.exists("_t.elf"))

    def testLoad(self):
        e = Elf.load("_t.elf")
        self.assertTrue("1" in e)
        self.assertTrue("10" in e)

    def tearDown(self):
        os.unlink("_t.elf")

class TestAddMany(unittest.TestCase):

    def setUp(self):
        self.e = Elf(100)

    def testAdd(self):
        self.e.addmany((str(i) for i in range(100)))

        for i in range(100):
            self.assertTrue(str(i) in self.e)

    def testUpdate(self):
        self.e.update((str(i) for i in range(100)))

        for i in range(100):
            self.assertTrue(str(i) in self.e)

if __name__ == "__main__":
    unittest.main()


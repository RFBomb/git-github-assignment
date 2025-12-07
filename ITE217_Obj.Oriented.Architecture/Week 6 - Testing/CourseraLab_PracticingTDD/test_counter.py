
"""
Test Cases for Counter Web Service
"""
from unittest import TestCase
import status
from counter import app

class CounterTest(TestCase):
    """Test Cases for Counter Web Service"""

    def setUp(self):
        self.client = app.test_client()

    def test_create_a_counter(self):
        """It should create a counter"""
        result = self.client.post("/counters/foo")
        self.assertEqual(result.status_code, status.HTTP_201_CREATED)
        data = result.get_json()
        self.assertIn("foo", data)
        self.assertEqual(data["foo"], 0)

    def test_duplicate_counter(self):
        """It should return an error for duplicates"""
        result = self.client.post("/counters/bar")
        self.assertEqual(result.status_code, status.HTTP_201_CREATED)
        result = self.client.post("/counters/bar")
        self.assertEqual(result.status_code, status.HTTP_409_CONFLICT)

    def test_read_counter(self):
        """The service must be able to get a counter’s current value."""
        # returns value by default when accessed by name
        counterName = 'bar'
        result = self.client.get(f"/counters/{counterName}")
        self.assertEqual(result.status_code, status.HTTP_200_OK)
        data = result.get_json()
        self.assertEqual(data[counterName], 0)

        # returns an error when name not found
        result = self.client.get(f"/counters/invalidName")
        self.assertEqual(result.status_code, status.HTTP_404_NOT_FOUND)
        pass
    
    def test_update_counter(self):
        """The service must be able to update a counter by name."""
        counterName = 'updateTest'

        # returns an error when name not found
        result = self.client.put(f"/counters/{counterName}")
        self.assertEqual(result.status_code, status.HTTP_404_NOT_FOUND)

        self.client.post(f"/counters/{counterName}")
        # update the value to 9
        for i in range(1,10):
            result = self.client.put(f"/counters/{counterName}")
            self.assertEqual(result.status_code, status.HTTP_200_OK, "\n\n!! -- Did not receive STATUS OK -- ")
            data = result.get_json()
            self.assertEqual(i, data[counterName], f"\n\n-- expected {i}, received {data[counterName]}--")

        # read the final value
        result = self.client.get(f"/counters/{counterName}")
        self.assertEqual(result.status_code, status.HTTP_200_OK)
        data = result.get_json()
        self.assertEqual(data[counterName], 9)
        

    def test_delete_counter(self):
        """The service must be able to delete a counter."""
        counterName = 'deleteTest'
        self.client.post(f"/counters/{counterName}") # create the counter
        result = self.client.delete(f"/counters/{counterName}") # delete the counter
        self.assertEqual(result.status_code, status.HTTP_204_NO_CONTENT)

        result = self.client.delete(f"/counters/{counterName}") # delete the counter again (should not exists)
        self.assertEqual(result.status_code, status.HTTP_204_NO_CONTENT)
        pass


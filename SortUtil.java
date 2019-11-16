package assignment5;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Random;

/**
 * 
 * @author Eli Hebdon u0871009, Jakob Horvath u1092049
 * 
 *         Implements a generic mergesort and quicksort, with varying array
 *         cases (best, average, worst) Insertion sort is also used in
 *         conjunction with mergesort once it reaches the threshold value
 *
 */
public class SortUtil {

	// Tells the mergeSortRecursive method when to switch to insertion sort
	private static int threshold = 10;

	/**
	 * The driver method for mergeSortRecursive Creates a temp array with enough
	 * space to hold all of the arr values
	 * 
	 * @param arr -- array to be sorted
	 * @param cmp -- generic comparator to handle sorting
	 */
	public static <T> void mergesort(ArrayList<T> arr, Comparator<? super T> cmp) {
		ArrayList<T> temp = new ArrayList<T>(arr.size());
		for (int i = 0; i < arr.size(); i++)
			temp.add(null);

		mergeSortRecursive(arr, temp, 0, arr.size() - 1, cmp);
	}

	/**
	 * Receives necessary values from mergesort and recursively splits the array in
	 * half until the threshold is reached
	 * 
	 * @param arr   -- array to be sorted
	 * @param temp  -- temporary array to store sorted values
	 * @param start -- the first index in the sub-array
	 * @param end   -- the last index in the sub-array
	 * @param cmp   -- generic comparator to handle sorting
	 */
	public static <T> void mergeSortRecursive(ArrayList<T> arr, ArrayList<T> temp, int start, int end,
			Comparator<? super T> cmp) {

		if (end - start <= threshold) {
			insertionSort(arr, start, end, cmp);

		} else {
			int mid = start + (end - start) / 2;
			mergeSortRecursive(arr, temp, start, mid, cmp); // left half
			mergeSortRecursive(arr, temp, mid + 1, end, cmp); // right half
			Merge(arr, temp, start, mid + 1, end, cmp);
		}

	}

	/**
	 * Combines the two inputed sub-arrays into a whole sorted portion in temp
	 * Copies the sorted values back into arr
	 * 
	 * @param arr   -- array to be sorted
	 * @param temp  -- temporary array for holding sorted elements
	 * @param start -- first index in sub-array
	 * @param mid   -- ending index for first half of sub-array, first index for
	 *              second half of sub-array
	 * @param end   -- last index of sub-array
	 * @param cmp   -- generic comparator to handle sorting
	 */
	public static <T> void Merge(ArrayList<T> arr, ArrayList<T> temp, int start, int mid, int end,
			Comparator<? super T> cmp) {
		int i1 = start, i2 = mid;
		int index = start;

		while (i1 < mid && i2 <= end) {
			if (cmp.compare(arr.get(i1), arr.get(i2)) <= 0) {
				temp.set(index, arr.get(i1));
				i1++;
			}
			else {
				temp.set(index, arr.get(i2));
				i2++;
			}
			index++;
		}
		while (i1 < mid) {
			temp.set(index, arr.get(i1));
			i1++;
			index++;
		}
		while (i2 <= end) {
			temp.set(index, arr.get(i2));
			i2++;
			index++;
		}

		for (int i = start; i <= end; i++) {
			arr.set(i, temp.get(i));
		}

	}

	/**
	 * Sorts the remaining elements from mergesort using the insertion sort method
	 * Is called on a threshold of values which can be altered
	 * @param arr -- array to be sorted
	 * @param start -- first index of sub-array
	 * @param end -- last index of sub-array
	 * @param cmp -- generic comparator to handle sorting
	 */
	public static <T> void insertionSort(ArrayList<T> arr, int start, int end, Comparator<? super T> cmp) {
		for (int i = start + 1; i <= end; i++) {
			T toInsert = arr.get(i);
			for (int j = i; j > start; j--) {
				if (cmp.compare(toInsert, arr.get(j - 1)) > 0)
					break;
				Collections.swap(arr, j, j - 1);

			}
		}
	}
	
	/**
	 * The driver method for quicksort. Calls partition with beginning and ending indexes
	 * @param arr -- array to be sorted
	 * @param cmp -- generic comparator to handle sorting
	 */
	public static <T> void quicksort(ArrayList<T> arr, Comparator<? super T> cmp) {
		partition(arr, 0, arr.size() - 1, cmp);
	}

	/**
	 * partitions the input array by choosing a pivot. Pivots are chosen by calling one of three helper methods.
	 * Recursively splits the array by reassigning start and end values while start index is less than R and 
	 * end index is greater than L. Swaps L and R when L <= R.
	 * @param arr -- array to be sorted
	 * @param start -- first index of sub-array
	 * @param end -- last index of sub-array
	 * @param cmp -- generic comparator to handle sorting
	 */
	public static <T> void partition(ArrayList<T> arr, int start, int end, Comparator<? super T> cmp) {
		T pivot = getMedianPivot(arr, start, end, cmp);
		int L = start;
		int R = end;
		while (L <= R) {
			while (cmp.compare(arr.get(L), pivot) < 0)
				L++;
			while (cmp.compare(arr.get(R), pivot) > 0)
				R--;
			if (L <= R) {
				swap(arr, L, R);
				L++;
				R--;
			}
		}
		if (start < R) {
			partition(arr, start, R, cmp);
		}
		if (end > L) {
			partition(arr, L, end, cmp);
		}

	}

	/**
	 * swaps the elemnts at indexs L and R in the input array
	 * @param arr - given array
	 * @param L - first index 
	 * @param R - second index
	 */
	public static <T> void swap(ArrayList<T> arr, int L, int R) {
		T temp = arr.get(L);
		arr.set(L, arr.get(R));
		arr.set(R, temp);
	}

	/**
	 * returns a generic pivot located in the middle of start and end indexs of the given array 
	 * @param arr -- given array
	 * @param start -- first index of sub-array
	 * @param end -- last index of sub-array
	 * @param cmp -- generic comparator to handle sorting
	 * @return -- T element in arr
	 */
	public static <T> T getMiddlePivot(ArrayList<T> arr, int start, int end, Comparator<? super T> cmp) {
		return arr.get(start + (end - start) / 2);
	}

	/**
	 * returns a generic pivot located at the start index of the input array
	 * @param arr -- given array
	 * @param start -- beginning index of sub-array
	 * @return -- T beginning element in arr
	 */
	public static <T> T getFirstPivot(ArrayList<T> arr, int start) {
		return arr.get(start);
	}

	/**
	 * returns a generic pivot by sampling three random indexes in given sub-array and returning median element from sample indexes
	 * @param arr -- given array
	 * @param start -- first index of sub-array
	 * @param end -- last index of sub-array
	 * @param cmp -- generic comparator to handle sorting
	 * @return -- T element in arr
	 */
	public static <T> T getMedianPivot(ArrayList<T> arr, int start, int end, Comparator<? super T> cmp) {
		ArrayList<T> candidates = new ArrayList<T>();
		//adds elements at start, middle, and end of sub-array to candidates
	    candidates.add(arr.get(start));
	    candidates.add(arr.get(start + (end-start) /2));
	    candidates.add(arr.get(end));

	    //sort candidates
	    insertionSort(candidates, 0, 2, cmp);
		//return median candidate
		return candidates.get(1);

	}
	/**
	 * returns an ArrayList of Integers from 1 to size in ascending order
	 * @param size - length of returned ArrayList
	 * @return -- ArrayList
	 */
	public static ArrayList<Integer> generateBestCase(int size) {
		ArrayList<Integer> arr = new ArrayList<Integer>(size);
		for (int i = 1; i < size + 1; i++) {
			arr.add(i);

		}
		return arr;
	}

	/**
	 * returns an ArrayList of Integers in range 1 to size in random order
	 * @param size - length of returned ArrayList
	 * @return -- ArrayList
	 */
	public static ArrayList<Integer> generateAverageCase(int size) {
		ArrayList<Integer> arr = new ArrayList<Integer>(size);
		for (int i = 1; i < size + 1; i++) {
			arr.add(i);

		}
		//randomly swap elements in the array size times
		Random rand = new Random();
		for (int i = 0; i < size; i++) {
			Collections.swap(arr, i, rand.nextInt(size));
		}
		return arr;
	}


	/**
	 * returns an ArrayList of Integers in range 1 to size in descending order
	 * @param size - length of returned ArrayList
	 * @return -- ArrayList
	 */
	public static ArrayList<Integer> generateWorstCase(int size) {
		ArrayList<Integer> arr = new ArrayList<Integer>(size);
		for (int i = size; i > 0; i--) {
			arr.add(i);

		}
		return arr;
	}

}

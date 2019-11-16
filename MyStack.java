package assignment7;

import assignment6.MyLinkedList;

import java.util.NoSuchElementException;

/**
 * Represents a generic stack (first in, last out).
 * 
 * @author Jakob Hovath, Sabrina Walton
 * 
 * @param <T>
 *            -- the type of elements contained in the stack
 */
public class MyStack<T> {

	private MyLinkedList<T> stack;

	public MyStack() {
		stack = new MyLinkedList<T>();
	}

	/**
	 * Removes all of the elements from the stack.
	 */
	public void clear() {
		stack.clear();
	}

	/**
	 * Returns true if the stack contains no elements.
	 */
	public boolean isEmpty() {
		return stack.isEmpty();
	}

	/**
	 * Returns the item at the top of the stack without removing it from the
	 * stack. Throws NoSuchElementException if the stack is empty.
	 */
	public T peek() throws NoSuchElementException {
		return stack.getFirst();
	}

	/**
	 * Returns and removes the item at the top of the stack. Throws
	 * NoSuchElementException if the stack is empty.
	 */
	public T pop() throws NoSuchElementException {
		return stack.removeFirst();
	}

	/**
	 * Pushes the input item onto the top of the stack.
	 */
	public void push(T item) {
		stack.addFirst(item);
	}

	/**
	 * Returns the number of items in the stack.
	 */
	public int size() {
		return stack.size();
	}
}


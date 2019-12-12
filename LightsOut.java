package a9;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Random;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.UIManager;

/**
 * 
 * @author Jakob Horvath
 *
 */
public class LightsOut extends JPanel implements ActionListener {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private static JFrame gameWindow;
	private static LightButton buttons[][];
	private JButton restart;
	private JButton quit;
	private JButton hint;
	private JTextArea hintText;
	private static JTextArea counter;
	private static int turnCount;
	private static int lightCount;

	/**
	 * initiates instance variables and assigns them certain properties
	 */
	public LightsOut() {
		super();

		restart = new JButton("Restart");
		restart.addActionListener(this);

		quit = new JButton("Quit");
		quit.addActionListener(this);

		hint = new JButton("Hint");
		hint.addActionListener(this);

		hintText = new JTextArea();
		hintText.setBackground(Color.getHSBColor(195, 29, 91));
		hintText.setFont(new Font("Dialog", Font.PLAIN, 16));
		hintText.setLineWrap(true);
		hintText.setWrapStyleWord(true);

		counter = new JTextArea();
		counter.setBackground(Color.getHSBColor(195, 29, 91));
		counter.setFont(new Font("Dialog", Font.PLAIN, 82));
		counter.setEditable(false);

		lightCount = 0;
		turnCount = 0;

		GridLayout grid = new GridLayout(6, 5); // grid has extra row to incorporate menu buttons
		this.setLayout(grid);

		buttons = new LightButton[5][5]; // new 2D array

		for (int row = 0; row < buttons.length; row++) {
			for (int col = 0; col < buttons[0].length; col++) {
				LightButton b = new LightButton();
				buttons[row][col] = b; // add button to 2D array
				this.add(b);
				b.addActionListener(this); // assign Action Listener to button
			}
		}

		// add other buttons to the last row of the grid
		this.add(counter);
		this.add(restart);
		this.add(quit);
		this.add(hint);
		this.add(hintText);

	}

	/**
	 * responsible for deciding what actions should be taken when a particular
	 * button is clicked
	 */
	public void actionPerformed(ActionEvent e) {
		LightButton b = null;
		if (e.getSource() instanceof LightButton) { // if button is part of the game board
			b = (LightButton) e.getSource();
			b.lightSwitch();
			hintText.setText(""); // remove last hint
			turnCount++; // increment turn count
			counter.setText("" + turnCount); // display turn count

			for (int row = 0; row < buttons.length; row++) { // attempt to switch adjacent lights to the one clicked
				for (int col = 0; col < buttons[0].length; col++) {
					if (b == buttons[row][col]) {
						if (row != 4) // have to avoid IndexOutOfBoundsException
							buttons[row + 1][col].lightSwitch();
						if (row != 0)
							buttons[row - 1][col].lightSwitch();
						if (col != 4)
							buttons[row][col + 1].lightSwitch();
						if (col != 0)
							buttons[row][col - 1].lightSwitch();
					}
				}
			}

			if (winCondition() == 0) { // all lights are turned off
				for (int row = 0; row < buttons.length; row++) {
					for (int col = 0; col < buttons[0].length; col++) {
						buttons[row][col].setEnabled(false); // disable game board buttons
					}
				}
				celebrate(); // randomize button colors

				buttons[2][2].winButton(); // display win button
			}

		}

		else if (e.getSource() == hint) { // hint button is clicked
			giveHint();
		}

		else if (e.getSource() == quit) { // quit button is clicked
			System.exit(0);
		}

		else { // restart button is clicked
			setBoard();
		}

	}

	/**
	 * create a new, randomized game board
	 */
	public static void setBoard() {
		LightsOut game = new LightsOut();
		gameWindow.setContentPane(game);
		gameWindow.setPreferredSize(new Dimension(700, 700)); // game window dimensions

		// perform a random number of clicks to create unique game board
		Random rand = new Random();
		LightButton b; // set new button
		for (int i = 0; i < rand.nextInt(5) + 5; i++) {
			b = buttons[rand.nextInt(4)][rand.nextInt(4)]; // choose random button on game board
			b.doClick(); // simulate mouse click
		}

		if (winCondition() == 0) // avoid accidental win screens after the randomized clicks
			setBoard(); // retry board setup

		turnCount = 0; // reset turn count, since previous clicks were made to create a random board
		counter.setText("" + turnCount);

		gameWindow.pack();
		gameWindow.setVisible(true);
	}

	/**
	 * decides whether to give a hint, as well as what particular hint to give
	 */
	public void giveHint() {
		int l1 = 0, l2 = 0, l3 = 0, l4 = 0, l5 = 0;
		boolean hintAvailable = true;
		for (int row = 0; row < buttons.length - 1; row++) { // loop through the first 4 rows of game buttons
			for (int col = 0; col < buttons[0].length; col++) {
				if (buttons[row][col].checkIsOn() == true) { // if any are still on...
					hintAvailable = false; // no hints may be given
					break;
				}
			}
		}

		if (hintAvailable == true) { // hint can be given (all lights are turned off except in the last row)
			for (int lastRow = 4; lastRow < buttons.length; lastRow++) {
				for (int col_ = 0; col_ < buttons[0].length; col_++) {
					// must check which particular lights are still on
					if (col_ == 0 & buttons[lastRow][col_].checkIsOn() == true)
						l1 = 1;
					if (col_ == 1 & buttons[lastRow][col_].checkIsOn() == true)
						l2 = 1;
					if (col_ == 2 & buttons[lastRow][col_].checkIsOn() == true)
						l3 = 1;
					if (col_ == 3 & buttons[lastRow][col_].checkIsOn() == true)
						l4 = 1;
					if (col_ == 4 & buttons[lastRow][col_].checkIsOn() == true)
						l5 = 1;
				}
			}
		}

		else // no hint given
			hintText.setText("Sorry, only have lights in the bottom row before asking for a hint :)");

		if (hintAvailable == true) { // give correct hint based on which lights in the bottom row are still on
			if (l1 == 1 && l5 == 1 && l2 == 0)
				hintText.setText("On the top row, click on the first and second light and work your way down");
			else if (l2 == 1 && l4 == 1 && l1 == 0)
				hintText.setText("On the top row, click on the first and fourth light and work your way down");
			else if (l1 == 1 && l2 == 1 && l3 == 1)
				hintText.setText("On the top row, click on the second light and work your way down");
			else if (l3 == 1 && l4 == 1 && l5 == 1)
				hintText.setText("On the top row, click on the fourth light and work your way down");
			else if (l1 == 1 && l3 == 1 && l4 == 1)
				hintText.setText("On the top row, click on the fifth light and work your way down");
			else if (l2 == 1 && l3 == 1 && l5 == 1)
				hintText.setText("On the top row, click on the first light and work your way down");
			else if (l1 == 1 && l2 == 1 && l4 == 1 && l5 == 1)
				hintText.setText("On the top row, click on the third light and work your way down");
		}

	}

	/**
	 * checks to see if win condition is true (0 lights on)
	 * 
	 * @return total number of lights still on
	 */
	public static int winCondition() {
		lightCount = 0;
		for (int row = 0; row < buttons.length; row++) { // loop through 2D array of buttons
			for (int col = 0; col < buttons[0].length; col++) {
				if (buttons[row][col].checkIsOn() == true) // if light is on
					lightCount++; // increment light count
			}
		}
		return lightCount;
	}

	/**
	 * calls celebration method in LightButton class to create a colorful layout of
	 * buttons
	 */
	public void celebrate() {
		for (int row = 0; row < buttons.length; row++) {
			for (int col = 0; col < buttons[0].length; col++) {
				buttons[row][col].celebration();
			}
		}
	}

	/**
	 * initiates the first game
	 * 
	 * @param args
	 */
	public static void main(String[] args) {
		gameWindow = new JFrame("Lights Out");
		gameWindow.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		try {
			UIManager.setLookAndFeel(UIManager.getCrossPlatformLookAndFeelClassName());
		} catch (Exception e) {
			e.printStackTrace();
		}

		setBoard(); // set up game board
	}

}

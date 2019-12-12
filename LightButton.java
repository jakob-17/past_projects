package a9;

import java.awt.Color;
import java.awt.Font;
import java.util.Random;

import javax.swing.JButton;

/**
 * 
 * @author Jakob Horvath
 *
 */

public class LightButton extends JButton {

	private static final long serialVersionUID = 1L;

	private boolean isOn; // determines state of game buttons

	/**
	 * initiates the game buttons
	 */
	public LightButton() {
		super();

		this.setBackground(Color.black); // set all game buttons initially to black
		isOn = false;
	}

	/**
	 * switches light to opposite state / color
	 */
	public void lightSwitch() {
		if (isOn == true) {
			this.setBackground(Color.black);
			isOn = false;
		} else if (isOn == false) {
			this.setBackground(Color.white);
			isOn = true;
		}

	}

	/**
	 * checks to see if light (button) is on / off
	 * 
	 * @return true / false
	 */
	public boolean checkIsOn() {
		return isOn;
	}

	/**
	 * alters center game button to display a game winning scene
	 */
	public void winButton() {
		this.setBackground(Color.black);
		this.setText("You Win!");
		this.setFont(new Font("Dialog", Font.PLAIN, 24));
	}

	/**
	 * alters game buttons to adopt a randomly assigned color and display along with
	 * the win button
	 */
	public void celebration() {
		Random rand = new Random();
		this.setBackground(Color.getHSBColor(rand.nextInt(255), rand.nextInt(255), rand.nextInt(255)));
	}
}

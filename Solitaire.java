package solitaire;

import java.io.IOException;
import java.util.Scanner;
import java.util.Random;
import java.util.NoSuchElementException;

/**
 * This class implements a simplified version of Bruce Schneier's Solitaire Encryption algorithm.
 * 
 * @author RU NB CS112
 */
public class Solitaire {
	
	/**
	 * Circular linked list that is the deck of cards for encryption
	 */
	CardNode deckRear;
	
	/**
	 * Makes a shuffled deck of cards for encryption. The deck is stored in a circular
	 * linked list, whose last node is pointed to by the field deckRear
	 */
	public void makeDeck() {
		
		// start with an array of 1..28 for easy shuffling
		int[] cardValues = new int[28];
		// assign values from 1 to 28
		for (int i=0; i < cardValues.length; i++) {
			cardValues[i] = i+1;
		}
		
		// shuffle the cards
		Random randgen = new Random();
        for (int i = 0; i < cardValues.length; i++) {
            int other = randgen.nextInt(28);
            int temp = cardValues[i];
            cardValues[i] = cardValues[other];
            cardValues[other] = temp;
        }
	     
	    // create a circular linked list from this deck and make deckRear point to its last node
	    CardNode cn = new CardNode();
	    cn.cardValue = cardValues[0];
	    cn.next = cn;
	    deckRear = cn;
	    for (int i=1; i < cardValues.length; i++) {
	    	cn = new CardNode();
	    	cn.cardValue = cardValues[i];
	    	cn.next = deckRear.next;
	    	deckRear.next = cn;
	    	deckRear = cn;
	    }
	}
	
	/**
	 * Makes a circular linked list deck out of values read from scanner.
	 */
	public void makeDeck(Scanner scanner)
	throws IOException {
		CardNode cn = null;
		if (scanner.hasNextInt()) {
			cn = new CardNode();
		    cn.cardValue = scanner.nextInt();
		    cn.next = cn;
		    deckRear = cn;
		}
		while (scanner.hasNextInt()) {
			cn = new CardNode();
	    	cn.cardValue = scanner.nextInt();
	    	cn.next = deckRear.next;
	    	deckRear.next = cn;
	    	deckRear = cn;
		}
	}
	
	/**
	 * Implements Step 1 - Joker A - on the deck.
	 */
	void jokerA() {
		CardNode tmp = deckRear;
		
		do{
			if (tmp.next.cardValue == 27){
				CardNode a = tmp; //card before 27
				CardNode b = tmp.next; //27
				CardNode c = tmp.next.next; //card after 27
				CardNode d = tmp.next.next.next; //two cards after 27
				a.next = c;
				c.next = b;
				b.next = d;
				

				if (deckRear.cardValue == 27){
					deckRear = c;
				}
				else if (deckRear == c){
					deckRear = b;
				}

				return;
			}
			else{
				tmp = tmp.next;
			}			
		}while (tmp != deckRear);
		

	}
	
	/**
	 * Implements Step 2 - Joker B - on the deck.
	 */
	void jokerB() {
		CardNode tmp = deckRear;
		
		do{
			if (tmp.next.cardValue == 28){
				CardNode a = tmp; //card before 28
				CardNode b = tmp.next; //28
				CardNode c = tmp.next.next; //card after 28
				CardNode d = tmp.next.next.next; //two cards after 28
				CardNode e = tmp.next.next.next.next; //three cards after 28
				a.next = c;
				c.next = d;
				d.next = b;
				b.next = e;
				if (deckRear.cardValue == 28){
					deckRear = c;
				}
				
				if (deckRear == d){
					deckRear = b;
				}

				return;
				
			}
			else{
				tmp = tmp.next;
			}
		}while(tmp != deckRear);
		
		return;
	}
	
	/**
	 * Implements Step 3 - Triple Cut - on the deck.
	 */
	void tripleCut() {
		// partition the deck into three sections - the section before the jokers (A),
		//  the section beginning and ending with jokers (J), and the section after the jokers (B) -
		//  by finding the nodes that start and end each section
		CardNode a1 = deckRear.next, a2, j1, j2, b1, b2 = deckRear;
		a2 = j1 = j2 = b1 = null;
		
		CardNode tmp = deckRear;
		
		do {
			if (tmp.next.cardValue == 27 || tmp.next.cardValue == 28){
				a2 = tmp;
				j1 = tmp.next;
				tmp = tmp.next.next;	// skip ahead before searching for next joker
				break;
			}
			tmp = tmp.next;
		} while (tmp != deckRear);
		
		do {
			if (tmp.cardValue == 27 || tmp.cardValue == 28){
				j2 = tmp;
				b1 = tmp.next;
				break;
			}
			tmp = tmp.next;
		} while (tmp != deckRear.next);
		
		if (j1 == deckRear.next && j2 == deckRear){
			return;
		} else if (j1 == deckRear.next){
			deckRear = j2;
			j2.next = b1;
			b2.next = j1;
		} else if (j2 == deckRear){
			deckRear = a2;
			a2.next = j1;
			j2.next = a1;
		} else {
			deckRear = a2;
			a2.next = b1;
			b2.next = j1;
			j2.next = a1;
		}

	}
	
	/**
	 * Implements Step 4 - Count Cut - on the deck.
	 */
	void countCut() {		
		
		int num = deckRear.cardValue; //number of cards to move
		
		if (num == 28 || num == 27){
			return;
		}

		CardNode tmp = deckRear;
		CardNode prev = deckRear;
		
		//finds card before deckRear
		do{
			if (tmp.next == deckRear){
				prev = tmp;
				break;
			}
			tmp = tmp.next;
		}while (tmp != deckRear);
		
		//beginning card to move
		CardNode m1 = deckRear.next;

		//finds last card to move
		CardNode m2 = deckRear;
		for (int i = 0; i < num; i++){
			m2 = m2.next;
		}
		
		//reassign pointers
		deckRear.next = m2.next;
		m2.next = deckRear;
		prev.next = m1;
		

	}
	
        /**
         * Gets a key. Calls the four steps - Joker A, Joker B, Triple Cut, Count Cut, then
         * counts down based on the value of the first card and extracts the next card value 
         * as key, but if that value is 27 or 28, repeats the whole process until a value
         * less than or equal to 26 is found, which is then returned.
         * 
         * @return Key between 1 and 26
         */
	int getKey() {
		
		jokerA();
		jokerB();
		tripleCut();
		countCut();
		
		//how many spaces to count down
		int num = deckRear.next.cardValue;
		
		if(num == 28){
			num = 27;
		}
		
		CardNode tmp = deckRear;
		
		//finds key (card after the first cards num value)
		for (int i = 0; i <= num; i++){
			tmp = tmp.next;
		}
		
		if (tmp.cardValue == 28 || tmp.cardValue == 27){
			getKey();
		}
		
	    return tmp.cardValue;
	}
	
	/**
	 * Utility method that prints a circular linked list, given its rear pointer
	 * 
	 * @param rear Rear pointer
	 */
	private static void printList(CardNode rear) {
		if (rear == null) { 
			return;
		}
		System.out.print(rear.next.cardValue);
		CardNode ptr = rear.next;
		do {
			ptr = ptr.next;
			System.out.print("," + ptr.cardValue);
		} while (ptr != rear);
		System.out.println("\n");
	}

	/**
	 * Encrypts a message, ignores all characters except upper case letters
	 * 
	 * @param message Message to be encrypted
	 * @return Encrypted message, a sequence of upper case letters only
	 */
	public String encrypt(String message) {	
		
		String result = "";

		int length = message.length();
		
		for (int i = 0; i < length; i++){
			
			char ch = message.charAt(i);
			
			//find letter, make it uppercase
			if (Character.isLetter(ch)){
				
				ch = Character.toUpperCase(ch);
				
				//find alphabet position
				int c = ch-'A'+1;
				
				//sum alphabet and key
				int sum = c + getKey();

				if (sum > 26){
					sum = sum - 26;
				}
				
				//finds letter
				ch = (char)(sum-1+'A');
				
				//add to result
				result = result + ch;
			}

		}
		
	    return result;
	}
	
	/**
	 * Decrypts a message, which consists of upper case letters only
	 * 
	 * @param message Message to be decrypted
	 * @return Decrypted message, a sequence of upper case letters only
	 */
	public String decrypt(String message) {	
		
		
		String result = "";
		int length = message.length();
		int c2 = 0;
		
		for (int i = 0; i < length; i++){
			
			//find alphabet position
			char ch = message.charAt(i);
			int c = ch-'A'+1;
			
			int k = getKey();
			//if code is <= key, add 26 to code
			if (c <= k){
				c = c + 26;
			}
			
			
			//subtract code from key
			c2 = c - k;

			//get letters
			ch = (char)(c2-1+'A');
			
			result = result + ch;
		}
	    return result;
	}
}

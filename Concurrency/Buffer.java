public class Buffer{

	double [] bb = new double[1000]; //at most 1000 items
	int pushIndex; // current slot
	int pushNext; //next slot (loops after 999)
	int popIndex; //same for pop
	int popNext;

	//constructor init to first slot
	public Buffer(){
		pushIndex = 0;
		pushNext = 1;
		popIndex = 0;
		popNext = 1;
		//-1 to represent avaialable slot.
		for (int i = 0; i< 1000; i++)
		{
			bb[i] = -1;
		}
	}

	//get and set methods.
	public void push(double element){
		bb[pushIndex] = element; //write the element
		pushIndex = pushNext; //increment index

		//increment pointer and loop if applicable
		pushNext = (pushNext == 999) ? 0 : pushNext+1;
	}
	public double pop()
	{
		double element = bb[popIndex]; //get the element
		bb[popIndex] = -1; //empty the slot
		popIndex = popNext; //increment index and loop if needed 
		popNext = (popNext == 999) ? 0 : popNext+1;

		//give the element
		return element;
	}

	//a special helper to see if theres something in the buffer or if its empty without changing it.
	public double check(String who){
		if(who == "push"){
			return bb[pushIndex];
		}else if(who == "pop"){
			return bb[popIndex];
		}else{
			return -99.0; //error handling
		}
	}

}
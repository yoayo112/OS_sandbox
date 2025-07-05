public class Consumer implements Runnable{
	public String name;
	private Buffer shared;//reference to shared bounded buffer

	//simple constructor
	public Consumer(String name, Buffer shared){
		this.name = name;
		this.shared = shared;
	}


	//synch and eat a variable from the buffer
	private double consume()
	{
		double element;
		//System.out.println(name + " doing work.");
		synchronized(this.shared){
			//enter
			double check = this.shared.check("pop");
			while(check == -1)
			{
				try{
					this.shared.wait();
				}catch(InterruptedException e)
				{
					System.out.println("Handle interrupt");
				}
				check = this.shared.check("pop");
			}

			//critical
			element = this.shared.pop(); // eat the next slot available
			//System.out.println(name + " popping "+element);

			//exit
			this.shared.notify();
		}
		return element;
	}

	//eat 100,000,000 items
	public void run()
	{
		double sum = 0;
		int items = 0;
		for (int j = 0; j < 10; j++)
		{
			for(int i=0; i < 100000; i++)
			{
				double e = consume();
				sum += e;
				items++;
			}
			System.out.printf("Consumer: Consumed %,d items, Cumulative value of consumed items=%.3f%n",items, sum);
		}
		System.out.println("Consumer: Finished consuming 1,000,000 items");
	}
}
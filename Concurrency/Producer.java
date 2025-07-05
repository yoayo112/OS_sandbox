import java.util.Random;

public class Producer implements Runnable{
	public String name;
	private  Buffer shared; //reference to shared bounded buffer

	//simple constructor
	public Producer(String name, Buffer shared){
		this.name = name;
		this.shared = shared;
	}

	//make a thing, synch and store it in the bounded buffer
	private double produce()
	{
		//System.out.println(name + " doing work.");
		//make a random element
		Random r = new Random();
		Double element = r.nextDouble() * 100.0;

		//waiting
		synchronized(this.shared){
			//Entry
			double check =  this.shared.check("push");
			while(check != -1)
			{
				try{
					this.shared.wait();
				}catch(InterruptedException e)
				{
					System.out.println("Handle interrupt");
				}
				check = this.shared.check("push");
			}

			//Critical
			this.shared.push(element); //use buffer to track its own slots
			//System.out.println(name + " pushing "+element);

			//Exit
			this.shared.notify();
		}
		return element;
	}

	//make 100,000,000 items
	public void run()
	{
		double sum = 0;
		int items = 0;
		for (int j = 0; j < 10; j++)
		{
			for(int i=0; i < 100000; i++)
			{
				double e = produce();
				sum += e;
				items++;
			}
			System.out.printf("Producer: Generated %,d items, Cumulative value of consumed items=%.3f%n",items, sum);
		}
		System.out.println("Producer: Finished generating 1,000,000 items");
	}
}
//Sky Vercauteren
//June 2025
//Operating Systems

public class ProducerConsumer {

	//main
	public static void main(String[] args){

		Buffer bb = new Buffer(); //bb == "bounded buffer"

		//System.out.println("Starting thread");

		Producer p1 = new Producer("p1", bb);
		Consumer c1 = new Consumer("c1", bb);

		Thread producerThread = new Thread(p1);
		Thread consumerthread = new Thread(c1);

		producerThread.start();
		consumerthread.start();

		//interrupting the threads
		try{
			producerThread.join();
			consumerthread.join();
		}catch(InterruptedException e){
			System.out.println("Caught interrupt");
		}
		System.out.println("Exiting!");
		return;
	}
}
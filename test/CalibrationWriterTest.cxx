

#include <CalibrationWriter.h>

int main()
{
	CalibrationWriter* writer = new CalibrationWriter();
	writer->openDocument("test.xml");
	
	for(int feb_id =  0; feb_id < 10; ++feb_id){
		writer->addFeb(feb_id); 
		for(int feb_hybrid_id = 0; feb_hybrid_id < 4; ++feb_hybrid_id){
			writer->addHybrid(feb_id, feb_hybrid_id); 
		}
	}

	for(int feb_id =  0; feb_id < 10; ++feb_id){
		for(int feb_hybrid_id = 0; feb_hybrid_id < 4; ++feb_hybrid_id){
			for(int apv = 0; apv < 5; ++apv){
				for(int channel = 0; channel < 128; ++channel){
					for(int sample_n = 0; sample_n < 6; ++sample_n){
						writer->writeBaseline(feb_id, feb_hybrid_id, apv, channel, sample_n, 3000.); 
						writer->writeNoise(feb_id, feb_hybrid_id, apv, channel, sample_n, 30.); 
					}
				}
			}
		}
	}
	
	
	writer->closeDocument();

	return 0; 	
}

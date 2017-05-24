all: apps testing sharpen edgedetect blur multrgb quantize saturate threshold interactivetools delete_log

rebuild:
	$(MAKE) -C ../ rebuild
	


apps:
	@ $(MAKE) -C ../

testing:
	@echo "_______ DOING TESTS _________"

sharpen:
	@echo "TESTING SHARPEN...."
	@echo `../MedImage/medimage a.jpg -sharpen 7 ./generated/sharpen_7.png` &> log.txt
	@echo `../MedImage/medimage ./correct/sharpen_7.png -compare ./generated/sharpen_7.png`


edgedetect:
	@echo "TESTING EDGEDETECT...."
	@echo `../MedImage/medimage a.jpg -edgedetect ./generated/edgedetect.png` &> log.txt
	@echo `../MedImage/medimage ./correct/a_edgedetect.png -compare ./generated/edgedetect.png`
blur:
	@echo "TESTING BLUR...."
	@echo `../MedImage/medimage a.jpg -blur 10 ./generated/blur_10.png` &> log.txt
	@echo `../MedImage/medimage ./correct/a_blur_10.png -compare ./generated/blur_10.png`
multrgb:
	@echo "TESTING MULTRGB...."
	@echo `../MedImage/medimage a.jpg -multrgb 0.6 0.7 0.8 ./generated/multrgb_0.6_0.7_0.8.png` &> log.txt
	@echo `../MedImage/medimage ./correct/a_multrgb_0.6_0.7_0.8.png -compare ./generated/multrgb_0.6_0.7_0.8.png`
quantize:
	@echo "TESTING QUANTIZE...."
	@echo `../MedImage/medimage a.jpg -quantize 6 ./generated/quantize_6.png` &> log.txt
	@echo `../MedImage/medimage ./correct/a_quantize_6.png -compare ./generated/quantize_6.png`
saturate:
	@echo "TESTING SATURATE...."
	@echo `../MedImage/medimage a.jpg -saturate 2.5 ./generated/saturate_2.5.png` &> log.txt
	@echo `../MedImage/medimage ./correct/a_saturate_2.5.png -compare ./generated/saturate_2.5.png`
threshold:
	@echo "TESTING THRESHOLD...."
	@echo `../MedImage/medimage a.jpg -threshold 0.6 ./generated/threshold_0.6.png` &> log.txt
	@echo `../MedImage/medimage ./correct/a_threshold_0.6.png -compare ./generated/threshold_0.6.png`

interactivetools:
	@echo "TESTING INTERACTIVE TOOLS...."
	@echo `./test` &> log.txt
	@echo "-----TESTING BLUR...."
	@echo `../MedImage/medimage ./correct/blur.png -compare ./generated/blur.png`
	@echo "-----TESTING CALPEN...."
	@echo `../MedImage/medimage ./correct/calpen.png -compare ./generated/calpen.png`
	@echo "-----TESTING ERASER...."
	@echo `../MedImage/medimage ./correct/eraser.png -compare ./generated/eraser.png`
	@echo "-----TESTING HIGHLIGHT...."
	@echo `../MedImage/medimage ./correct/highlight.png -compare ./generated/highlight.png`
	@echo "-----TESTING PEN...."
	@echo `../MedImage/medimage ./correct/pen.png -compare ./generated/pen.png`
	@echo "-----TESTING SPECIAL...."
	@echo `../MedImage/medimage ./correct/special.png -compare ./generated/special.png`
	@echo "-----TESTING SPRAYCAN...."
	@echo `../MedImage/medimage ./correct/spraycan.png -compare ./generated/spraycan.png`
	

delete_log:
	@ rm log.txt

clean:
	$(MAKE) -C ../ clean

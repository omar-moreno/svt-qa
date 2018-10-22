import SvtQAConf


p = SvtQAConf.Process()

p.libraries.append("libBaselineProcessor.so")
###############################
#          Processors         #
###############################

baseline = SvtQAConf.Processor("Baseline", "BaselineProcessor")


p.sequence = [baseline]

p.input_files = ['input.root']
p.output_files = ['output.root']

p.printProcess()

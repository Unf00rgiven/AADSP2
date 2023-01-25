//----------------------------------------
// NOTE:  THIS FILE IS TO BE EVALUATED, NOT IMPORTED BY ANOTHER GROOVY FILE!!!
//----------------------------------------
//----------------------------------------
// The imported board module has properties that represent the different
// configurable entities on the board. The set of those properties
// varies from board to board, but can include:
//   board.dsp - supports DSP comm
//   board.mcu - supports reading/writing the MCU registers
//   board.fpga- supports reading/writing FPGA registers
//   board.i2c - supports reading/writing to I2C devices on the board
//
// The board module also has a few methods to directly invoke actions in the
// CLIDE application editor:
//   board.console_message(message) - print a message to the console in CLIDE
//   board.loadPrekickstartMessages() - send all prekickstart messages for the
//           'current' concurrency to the DSP.
//           Returns a map of maps of exported parameter values.
//   board.loadPostkickstartMessages() - send all postkickstart messages for the
//           'current' concurrency to the DSP"
//   board.loadULDs(uldList) - slave-boot loads all ulds in the list
//   board.softReset() - issues a soft reset command to the DSP
//   board.loadSnapshot(definition,snapshotName,config) - loads the specified
//           snapshot name, which sets the config dictionary, and validates the
//           config dictionary against the definition dictionary
//-----------------------------------------

//----------------------------------------
// This dictionary variable defines the parameters that must be set by a
// snapshot. Each variable definition is one element of a map. Each map
// element is of the form
//    key : definitionTuple
// where
//    key is the index into the snapshot map
//    definitionTuple is a tuple of the form:
//       ( type, label, defaultValue, restrictions )
// restrictions is optional, and can take any of the following forms:
//    a map of {string:value pairs}, representing a combobox-like choice
//    a list of (lowest,highest) allowed numeric values
//----------------------------------------
import clide.groovy.board.Board
import groovy.util.Eval
import clide.boardconfig.BoardConfigHelper
import com.cirrus.cdm.device.CPSDevice;

import clide.boardconfig.xml.simulator.XmlClProject;
import clide.boardconfig.xml.simulator.XmlFile;
import clide.boardconfig.xml.simulator.XmlFileIOConfig;
import clide.boardconfig.xml.simulator.XmlMemoryConfig;
import clide.boardconfig.xml.simulator.XmlUldFile;
import clide.boardconfig.xml.simulator.XmlUldListWriter;


Map configParameterDefinitions()
{ 
    [
    //
    // Input parameters.
    //
    'input_channels_per_line'      :[ "Input Channels per Line", 2, ((1)..(8))],
    'input_delay'                  :[ "Input Delay", 0, ((0)..(10000))],
    'input_file_mode'              :[ "Input File Mode", 'PCM', ['PCM', 'WAVE', 'ASCII']],
    'input_justification'          :[ "Input Justification", 'Left', ['Left', 'Right']],
    'input_sample_format'          :[ "Input Sample Format", 'LittleEndian', ['LittleEndian', 'BigEndian']],
    'input_sample_size'            :[ "Input Sample Size", 32, [16, 24, 32]],
    'input_sample_rate'            :[ "Input Sample Rate", 48000, [16000, 22050,32000, 44100, 48000]],
    'input_file_channel_3800'      :[ "Input File Channel 0 (0x3800)", "", null ],
    'input_file_channel_3801'      :[ "Input File Channel 1 (0x3801)", "", null ],
    'input_file_channel_3802'      :[ "Input File Channel 2 (0x3802)", "", null ],
    'input_file_channel_3803'      :[ "Input File Channel 3 (0x3803)", "", null ],
    'input_file_channel_3804'      :[ "Input File Channel 4 (0x3804)", "", null ],
    'input_file_channel_3805'      :[ "Input File Channel 5 (0x3805)", "", null ],

    //
    // Output Parameters.
    //
    'output_channels_per_line'      :[ "Output Channels per Line", 2, ((1)..(8))],
    'output_delay'                  :[ "Output Delay", 0, ((0)..(10000))],
    'output_file_mode'              :[ "Output File Mode", "PCM", ['PCM', 'WAVE', 'ASCII']],
    'output_justification'          :[ "Output Justification", "Left", ['Left', 'Right']],
    'output_sample_format'          :[ "Output Sample Format", "LittleEndian", ["LittleEndian", "BigEndian"]],
    'output_sample_size'            :[ "Output Sample Size", 32, [16, 24, 32]],
    'output_sample_rate'            :[ "Input Sample Rate", 48000, [16000, 22050,32000, 44100, 48000]],
    'output_file_channel_3400'      :[ "Output File Channel 0 (0x3400)", "", null ],
    'output_file_channel_3401'      :[ "Output File Channel 1 (0x3401)", "", null ],
    'output_file_channel_3402'      :[ "Output File Channel 2 (0x3402)", "", null ],
    'output_file_channel_3403'      :[ "Output File Channel 3 (0x3403)", "", null ],
    'output_file_channel_3404'      :[ "Output File Channel 4 (0x3404)", "", null ],
    'output_file_channel_3405'      :[ "Output File Channel 5 (0x3405)", "", null ],

    //
    // Profiling
    //
    'profiling_support'             :[ "Profiling Support", "off", ["off":"", "on":"--profile"]],

    //
    // argv
    //
    'argv'                          :["Optional argv values","", null ]
    ]
}


/**
* This function performs all the necessary work to boot the DSP and configure the board.  
* This is the main entry point for the booting the board.
* 
* @param config
*        Configuration value
*
* @param uldList
*        A list of Strings that contains the full path for a uld file.
*/
void slave_boot(List uldList)
{
    //main slave boot procedure

	Map config = [   
    //
    // Input Parameters.
    //
    'input_channels_per_line':      2,
    'input_delay':                  0,
    'input_file_mode':              "PCM",
    'input_justification':          "Left",
    'input_sample_format':          "LittleEndian",
    'input_sample_size':            32,
    'input_sample_rate':            48000,
    'input_file_channel_3800':      "",
    'input_file_channel_3801':      "",
    'input_file_channel_3802':      "",
    'input_file_channel_3803':      "",
    'input_file_channel_3804':      "",
    'input_file_channel_3805':      "",

    //
    // Output Parameters.
    //
    'output_channels_per_line':     2,
    'output_delay':                 0,
    'output_file_mode':             "PCM",
    'output_justification':         "Left",
    'output_sample_format':         "LittleEndian",
    'output_sample_size':           32,
    'output_sample_rate':           48000,
    'output_file_channel_3400':     "",
    'output_file_channel_3401':     "",
    'output_file_channel_3402':     "",
    'output_file_channel_3403':     "",
    'output_file_channel_3404':     "",
    'output_file_channel_3405':     "",

    //
    // Profiling support
    //  
    'profiling_support':            "on",
	
	//
	// argv (optional)
	//
	'argv':                         ""
	]

    String[]  inputFileChannelKeyList = [
        'input_file_channel_3800',
        'input_file_channel_3801',
        'input_file_channel_3802',
        'input_file_channel_3803',
        'input_file_channel_3804',
        'input_file_channel_3805'
    ];

    String[]  outputFileChannelKeyList =
    [
        'output_file_channel_3400',
        'output_file_channel_3401',
        'output_file_channel_3402',
        'output_file_channel_3403',
        'output_file_channel_3404',
        'output_file_channel_3405'
    ];

    
    //
    // Create the jaxb items to write out for the simulator sbr configuration file.
    //
    XmlClProject    xmlRoot = new XmlClProject();
    XmlMemoryConfig memConfig  = new XmlMemoryConfig();
    XmlUldFile      uldFile = new XmlUldFile();
    XmlFileIOConfig fileInputConfig = new XmlFileIOConfig();
    XmlFileIOConfig fileOutputConfig = new XmlFileIOConfig();
    XmlFile fileInput; 
    int channel;
    
    //
    // Set up the links for the Root CL_PROJECT
    //
    xmlRoot.setMemoryConfig(memConfig);
    xmlRoot.addFileIOConfigList(fileInputConfig);
    xmlRoot.addFileIOConfigList(fileOutputConfig);
    
    //
    // Set up the links for MEMORY_CFG
    //
    memConfig.addUldFile(uldFile);
    
    //
    // Make the assumption that there is only one item in the list.
    //
    uldFile.diskPath                = uldList[0];
    
    
    
    //
    // Set up the links for Input FILE_IO_CFG
    //
    fileInputConfig.blockType       = 'Input'
    fileInputConfig.channelsPerLine = config['input_channels_per_line'];
    fileInputConfig.delay           = config['input_delay'].value;
    fileInputConfig.fileMode        = config['input_file_mode'];
    fileInputConfig.index           = 0;
    fileInputConfig.justification   = config['input_justification'];
    fileInputConfig.sampleFormat    = config['input_sample_format'];
    fileInputConfig.sampleRate      = config['input_sample_rate'];
    fileInputConfig.sampleSize      = config['input_sample_size'];
    
    //
    // Create the file channel list.
    //
    channel = 0;
    for(String fileChannelKey: inputFileChannelKeyList)
    {
        if(config[fileChannelKey].value)
        {
            fileInput = new XmlFile();
            fileInput.channel       = channel;
            fileInput.diskPath      = config[fileChannelKey];
			fileInputConfig.addFileItem(fileInput);
        }
		channel++;
    }
    
    //
    // Set up the links for Output FILE_IO_CFG
    //
    fileOutputConfig.blockType          = 'Output';
    fileOutputConfig.channelsPerLine    = config['output_channels_per_line'];
    fileOutputConfig.delay              = config['output_delay'];
    fileOutputConfig.fileMode           = config['output_file_mode'];
    fileOutputConfig.index              = 0;
    fileOutputConfig.justification      = config['output_justification'];
    fileOutputConfig.sampleFormat       = config['output_sample_format'];
    fileOutputConfig.sampleRate         = config['output_sample_rate'];
    fileOutputConfig.sampleSize         = config['output_sample_size'];
    
    
    //
    // Create the file channel list.
    //
    channel = 0;
    for(String fileChannelKey: outputFileChannelKeyList)
    {
		if(config[fileChannelKey])
		{
			if(config[fileChannelKey].value)
			{
				fileOutput              = new XmlFile();
				fileOutput.channel      = channel;
				fileOutput.diskPath     = config[fileChannelKey];
				fileOutputConfig.addFileItem(fileOutput);
			}
		}
		channel++;
    }
		//
		// set argv to config parameter
		//
		xmlRoot.argv = config['argv'];

    //
    // Find the java temp directory and create the sbr file there.
    //
    String sbrFile = System.getProperty("java.io.tmpdir") + "SimulatorConfigurationTemp.sbr";
    XmlUldListWriter.WriteSbrFile(sbrFile, xmlRoot);
	println sbrFile;
    
    //
    // Get the cps device
    //
    CPSDevice cpsDevice = board.getCpsDevice();
    
    //
    // Load the project
    //
    cpsDevice.DBILoadProject(sbrFile);
    
    //
    // Check to see if we need to enable profiling.
    //
    if(config['profiling_support'].toLowerCase() == "on")
    {
        cpsDevice.profilingEnable();
    }
    
    // Need to tell listeners that we are ready
    board.fireULDsLoadedEvent();
        
    board.getConsole().message("boot complete!\n")
}
 
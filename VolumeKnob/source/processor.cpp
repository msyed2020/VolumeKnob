//------------------------------------------------------------------------
// Copyright(c) 2025 My Plug-in Company.
//------------------------------------------------------------------------

#include "processor.h"
#include "cids.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"

using namespace Steinberg;

namespace MyCompanyName {
//------------------------------------------------------------------------
// VolumeKnobProcessor
//------------------------------------------------------------------------
VolumeKnobProcessor::VolumeKnobProcessor ()
{
	//--- set the wanted controller for our processor
	setControllerClass (kVolumeKnobControllerUID);
}

//------------------------------------------------------------------------
VolumeKnobProcessor::~VolumeKnobProcessor ()
{}

//------------------------------------------------------------------------
tresult PLUGIN_API VolumeKnobProcessor::initialize (FUnknown* context)
{
	// Here the Plug-in will be instantiated
	
	//---always initialize the parent-------
	tresult result = AudioEffect::initialize (context);
	// if everything Ok, continue
	if (result != kResultOk)
	{
		return result;
	}

	//--- create Audio IO ------
	addAudioInput (STR16 ("Stereo In"), Steinberg::Vst::SpeakerArr::kStereo);
	addAudioOutput (STR16 ("Stereo Out"), Steinberg::Vst::SpeakerArr::kStereo);

	/* If you don't need an event bus, you can remove the next line */
	addEventInput (STR16 ("Event In"), 1);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API VolumeKnobProcessor::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!
	
	//---do not forget to call parent ------
	return AudioEffect::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API VolumeKnobProcessor::setActive (TBool state)
{
	//--- called when the Plug-in is enable/disable (On/Off) -----
	return AudioEffect::setActive (state);
}

//------------------------------------------------------------------------
tresult PLUGIN_API VolumeKnobProcessor::process (Vst::ProcessData& data)
{
	//--- First : Read inputs parameter changes-----------

	float gain = 1.0f;
	
	

	if (data.inputParameterChanges)
	{
		auto* paramQueue = data.inputParameterChanges->getParameterData(kGainId);
		if (paramQueue && paramQueue->getPointCount() > 0)
		{
			int32 offset;
			Vst::ParamValue value;
			if (paramQueue->getPoint(paramQueue->getPointCount() - 1, offset, value) == kResultTrue)
				gain = static_cast<float>(value);
		}
	}

	

	if (data.numInputs == 1 && data.numOutputs == 1)
	{
		float** in = data.inputs[0].channelBuffers32;
		float** out = data.outputs[0].channelBuffers32;

		int32 numInChannels = data.inputs[0].numChannels;
		int32 numOutChannels = data.outputs[0].numChannels;
		int32 numChannels = std::min(numInChannels, numOutChannels);

		for (int32 channel = 0; channel < numChannels; ++channel)
		{
			for (int32 sample = 0; sample < data.numSamples; ++sample)
			{
				out[channel][sample] = in[channel][sample] * gain;
			}
		}
	}

	/*if (data.inputParameterChanges)
	{
		int32 numParamsChanged = data.inputParameterChanges->getParameterCount ();
		for (int32 index = 0; index < numParamsChanged; index++)
		{
			if (auto* paramQueue = data.inputParameterChanges->getParameterData (index))
			{
				Vst::ParamValue value;
				int32 sampleOffset;
				int32 numPoints = paramQueue->getPointCount ();
				switch (paramQueue->getParameterId ())
				{
				}
			}
		}
	}*/
	
	//--- Here you have to implement your processing

	if (data.numSamples > 0)
	{
		int32 minBus = std::min(data.numInputs, data.numOutputs);
		for (int32 i = 0; i < minBus; i++)
		{
			int32 minChan = std::min(data.inputs[i].numChannels, data.outputs[i].numChannels);
			for (int32 c = 0; c < minChan; c++)
			{
				float* in = data.inputs[i].channelBuffers32[c];
				float* out = data.outputs[i].channelBuffers32[c];

				for (int32 s = 0; s < data.numSamples; s++)
					out[s] = in[s] * gain;

				data.outputs[i].silenceFlags &= ~(1ULL << c); // not silent
			}

			for (int32 c = minChan; c < data.outputs[i].numChannels; c++)
			{
				memset(data.outputs[i].channelBuffers32[c], 0, data.numSamples * sizeof(Vst::Sample32));
				data.outputs[i].silenceFlags |= (1ULL << c);
			}
		}

		for (int32 i = minBus; i < data.numOutputs; i++)
		{
			for (int32 c = 0; c < data.outputs[i].numChannels; c++)
			{
				memset(data.outputs[i].channelBuffers32[c], 0, data.numSamples * sizeof(Vst::Sample32));
			}
			data.outputs[i].silenceFlags = ((uint64)1 << data.outputs[i].numChannels) - 1;
		}
	}

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API VolumeKnobProcessor::setupProcessing (Vst::ProcessSetup& newSetup)
{
	//--- called before any processing ----
	return AudioEffect::setupProcessing (newSetup);
}

//------------------------------------------------------------------------
tresult PLUGIN_API VolumeKnobProcessor::canProcessSampleSize (int32 symbolicSampleSize)
{
	// by default kSample32 is supported
	if (symbolicSampleSize == Vst::kSample32)
		return kResultTrue;

	// disable the following comment if your processing support kSample64
	/* if (symbolicSampleSize == Vst::kSample64)
		return kResultTrue; */

	return kResultFalse;
}

//------------------------------------------------------------------------
tresult PLUGIN_API VolumeKnobProcessor::setState (IBStream* state)
{
	// called when we load a preset, the model has to be reloaded
	IBStreamer streamer (state, kLittleEndian);
	
	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API VolumeKnobProcessor::getState (IBStream* state)
{
	// here we need to save the model
	IBStreamer streamer (state, kLittleEndian);

	return kResultOk;
}

//------------------------------------------------------------------------
} // namespace MyCompanyName

#ifndef INSDL_AUDIO
#define INSDL_AUDIO

#include <SDL3/SDL.h>
#include <iostream>
#include <string>
#include <utility>

namespace insdl {

/**
 * @brief Class for working with WAV audio files
 *
 * Allows loading WAV files, playing, pausing, resuming, and stopping audio
 * Uses SDL_AudioStream and SDL_AudioDevice to manage sound
 */
class audio {
    private:
        struct audioData {
            SDL_AudioSpec spec;
            SDL_AudioStream* audioStream = NULL;
            Uint8* wavData = NULL;
            Uint32 wavDataLen = 0;
            SDL_AudioDeviceID deviceId = 0;
            std::string filePath;
        };

        audioData data;
        std::string errorMessage;

        void setError(const std::string& context) {
            const char* sdlError = SDL_GetError();
            errorMessage = context + " failed";
            if (sdlError && sdlError[0] != '\0') {
                errorMessage += ": ";
                errorMessage += sdlError;
            }
        }
    public:
        /**
         * @brief Constructor, loads a WAV file and initializes the audio device
         * 
         * @param wavPath Path to the WAV file
         */
        audio(const std::string& wavPath) {
            data.filePath = wavPath;

            if (!SDL_LoadWAV(wavPath.c_str(), &data.spec, &data.wavData, &data.wavDataLen)) {
                setError("SDL_LoadWAV(\"" + wavPath + "\")");
                return;
            }

            data.audioStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &data.spec, NULL, NULL);
            if (!data.audioStream) {
                setError("SDL_OpenAudioDeviceStream(\"" + wavPath + "\")");
                return;
            }

            data.deviceId = SDL_GetAudioStreamDevice(data.audioStream);
            if (!data.deviceId) {
                setError("SDL_GetAudioStreamDevice(\"" + wavPath + "\")");
                return;
            }

            if (!SDL_ResumeAudioDevice(data.deviceId)) {
                setError("SDL_ResumeAudioDevice(\"" + wavPath + "\")");
            }
        }

        audio(const audio&) = delete;
        audio& operator=(const audio&) = delete;

        audio(audio&& other) noexcept
            : data(std::exchange(other.data, audioData{})) {}

        audio& operator=(audio&& other) noexcept {
            if (this != &other) {
                stop();
                data = std::exchange(other.data, audioData{});
            }
            return *this;
        }

        ~audio() {
            stop();
        }

        /**
         * @brief Returns true if the WAV file, audio stream and device were initialized successfully.
         */
        bool ok() const {
            return errorMessage.empty() && data.wavData && data.audioStream && data.deviceId;
        }

        /**
         * @brief Returns the last SDL-related error message.
         */
        std::string error() const {
            return errorMessage;
        }

        /**
         * @brief Plays the audio
         */
        void play() {
            if (!data.audioStream || !data.wavData) {
                errorMessage = "Cannot play audio: audio is not loaded";
                return;
            }
            if (SDL_GetAudioStreamQueued(data.audioStream) < (int)data.wavDataLen) {
                if (!SDL_PutAudioStreamData(data.audioStream, data.wavData, data.wavDataLen)) {
                    setError("SDL_PutAudioStreamData(\"" + data.filePath + "\")");
                }
            }
        }

        /**
         * @brief Resumes audio playback
         */
        void resume() {
            if (data.deviceId) {
                if (!SDL_ResumeAudioDevice(data.deviceId)) {
                    setError("SDL_ResumeAudioDevice(\"" + data.filePath + "\")");
                }
            } else {
                errorMessage = "Cannot resume audio: audio device is not open";
            }
        }

        /**
         * @brief Pauses audio playback
         */
        void pause() {
            if (data.deviceId) {
                if (!SDL_PauseAudioDevice(data.deviceId)) {
                    setError("SDL_PauseAudioDevice(\"" + data.filePath + "\")");
                }
            } else {
                errorMessage = "Cannot pause audio: audio device is not open";
            }
        }

        /**
         * @brief Stops playback and frees resources
         * 
         * Closes the device, destroys the stream, and frees WAV data
         */
        void stop() {
            if (data.deviceId) {
                SDL_PauseAudioDevice(data.deviceId);
                SDL_CloseAudioDevice(data.deviceId);
                data.deviceId = 0;
            }
            if (data.audioStream) {
                SDL_DestroyAudioStream(data.audioStream);
                data.audioStream = NULL;
            }
            if (data.wavData) {
                SDL_free(data.wavData);
                data.wavData = NULL;
            }
        }

        /**
         * @brief Returns the structure with audio data
         * 
         * @return audioData Structure with current audio parameters
         */
        audioData get() const {
            return data;
        }

        /**
         * @brief Overloaded output operator to display audio information
         * 
         * Outputs the file path
         */
        friend std::ostream& operator<<(std::ostream& os, const audio& a) {
            os << "Audio(path: \"" << a.data.filePath << "\")";
            return os;
        }
};

using Audio = audio;

} // namespace insdl

#endif

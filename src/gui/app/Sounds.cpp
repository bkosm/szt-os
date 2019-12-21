#include "Sounds.h"
#include "Definitions.h"

Sounds::Sounds()
{
	boot_.loadFromFile(BOOT_SOUND_PATH);
	shutdown_.loadFromFile(EXIT_SOUND_PATH);
	error_.loadFromFile(ERROR_SOUND_PATH);

	boot.setBuffer(boot_);
	shutdown.setBuffer(shutdown_);
	error.setBuffer(error_);
}

void Sounds::setVolume(const float volume)
{
	boot.setVolume(volume);
	error.setVolume(volume);
	shutdown.setVolume(volume);
}

/** \file
 *  Game Develop
 *  2008-2011 Florian Rival (Florian.Rival@gmail.com)
 */

#include "AudioTools.h"
#include <string>
#include "GDL/RuntimeScene.h"

struct est_fini
{
    bool operator ()( sf::Sound &a ) const
    {
        if (a.GetStatus() == sf::Sound::Stopped )
        {
            return true;
        }

        return false;
    }
};

////////////////////////////////////////////////////////////
/// Joue simplement un son
///
/// Type : PlaySound
/// Param�tre 1 : Fichier son
/// Param�tre 2 : Bouclage ( facultatif )
/// Param�tre 3 : Volume ( facultatif )
////////////////////////////////////////////////////////////
bool ActPlaySound( RuntimeScene & scene, ObjectsConcerned & objectsConcerned, const Instruction & action )
{
    sf::Clock Latence;

    SoundManager * soundManager;
    soundManager = SoundManager::GetInstance();

    Son * son = new Son(action.GetParameter(0).GetPlainString());
    soundManager->sounds.push_back(son);
    soundManager->sounds.back()->sound.Play();

    //Compatibilit� avec Game Develop 1.0.1979 et inf�rieur
    //On verifie si l'argument 2 ( Bouclage ) existe
    if ( action.GetParameters().size() > 1 )
    {
        soundManager->sounds.back()->sound.SetLoop(action.GetParameter(1).GetAsBool());
    }

    //Compatibilit� avec Game Develop 1.1.5429 et inf�rieur
    //On verifie si l'argument 3 ( Volume ) existe
    if ( action.GetParameters().size() > 2 )
    {
        if ( !action.GetParameter(2).GetPlainString().empty() )
            soundManager->sounds.back()->SetVolume(action.GetParameter( 2 ).GetAsMathExpressionResult(scene, objectsConcerned));
        else
            soundManager->sounds.back()->SetVolume(100);
    }

    //Compatibility with Game Develop 1.5.9980 and below
    if ( action.GetParameters().size() > 3 )
    {
        if ( !action.GetParameter(3).GetPlainString().empty() )
            soundManager->sounds.back()->SetPitch(action.GetParameter( 3 ).GetAsMathExpressionResult(scene, objectsConcerned));
        else
            soundManager->sounds.back()->SetPitch(1);
    }

    scene.pauseTime += Latence.GetElapsedTime();

    return true;
}

////////////////////////////////////////////////////////////
/// Joue un son sur un canal
///
/// Type : PlaySoundCanal
/// Param�tre 1 : Fichier son
/// Param�tre 2 : Canal
/// Param�tre 3 : Bouclage ( fac )
/// Param�tre 4 : Volume ( fac )
////////////////////////////////////////////////////////////
bool ActPlaySoundCanal( RuntimeScene & scene, ObjectsConcerned & objectsConcerned, const Instruction & action )
{
    int canal = static_cast<int> ( action.GetParameter( 1 ).GetAsMathExpressionResult(scene, objectsConcerned) );
    if ( canal < 0 || canal > MAX_CANAUX_SON ) return false;

    //Chargement
    SoundManager * soundManager;
    soundManager = SoundManager::GetInstance();

    //Son � jouer
    Son * son = new Son(action.GetParameter(0).GetPlainString());
    son->sound.Play();

    soundManager->SetSoundOnChannel(canal, son);

    //Compatibilit� avec Game Develop 1.0.1979 et inf�rieur
    //On verifie si l'argument 3 ( Bouclage ) existe
    if ( action.GetParameters().size() > 2 )
    {
        soundManager->GetSoundOnChannel(canal)->sound.SetLoop(action.GetParameter(2).GetAsBool());
    }

    //Compatibilit� avec Game Develop 1.1.5429 et inf�rieur
    //On verifie si l'argument 4 ( Volume ) existe
    if ( action.GetParameters().size() > 3 )
    {
        if ( !action.GetParameter(3).GetPlainString().empty() )
            soundManager->GetSoundOnChannel(canal)->SetVolume(action.GetParameter( 3 ).GetAsMathExpressionResult(scene, objectsConcerned));
        else
            soundManager->GetSoundOnChannel(canal)->SetVolume(100);
    }

    //Compatibility with Game Develop 1.5.9980 and below
    if ( action.GetParameters().size() > 4 )
    {
        if ( !action.GetParameter(4).GetPlainString().empty() )
            soundManager->GetSoundOnChannel(canal)->SetPitch(action.GetParameter( 4 ).GetAsMathExpressionResult(scene, objectsConcerned));
        else
            soundManager->GetSoundOnChannel(canal)->SetPitch(1);
    }

    return true;
}


////////////////////////////////////////////////////////////
/// Stoppe un son sur un canal
///
/// Type : StopSoundCanal
/// Param�tre 1 : Canal
////////////////////////////////////////////////////////////
bool ActStopSoundCanal( RuntimeScene & scene, ObjectsConcerned & objectsConcerned, const Instruction & action )
{
    int canal = static_cast<int> ( action.GetParameter( 0 ).GetAsMathExpressionResult(scene, objectsConcerned));

    if ( canal < 0 || canal > MAX_CANAUX_SON ) return false;

    SoundManager::GetInstance()->GetSoundOnChannel(canal)->sound.Stop();

    return true;
}

////////////////////////////////////////////////////////////
/// Met en pause un son sur un canal
///
/// Type : PauseSoundCanal
/// Param�tre 1 : Canal
////////////////////////////////////////////////////////////
bool ActPauseSoundCanal( RuntimeScene & scene, ObjectsConcerned & objectsConcerned, const Instruction & action )
{
    int canal = static_cast<int> ( action.GetParameter( 0 ).GetAsMathExpressionResult(scene, objectsConcerned));
    if ( canal < 0 || canal > MAX_CANAUX_SON ) return false;

    SoundManager::GetInstance()->GetSoundOnChannel(canal)->sound.Pause();

    return true;
}

////////////////////////////////////////////////////////////
/// Re(joute) un son sur un canal
///
/// Type : RePlaySoundCanal
/// Param�tre 1 : Canal
////////////////////////////////////////////////////////////
bool ActRePlaySoundCanal( RuntimeScene & scene, ObjectsConcerned & objectsConcerned, const Instruction & action )
{
    int canal = static_cast<int> ( action.GetParameter( 0 ).GetAsMathExpressionResult(scene, objectsConcerned));
    if ( canal < 0 || canal > MAX_CANAUX_SON )  return false;

    SoundManager::GetInstance()->GetSoundOnChannel(canal)->sound.Play();

    return true;
}

////////////////////////////////////////////////////////////
/// Joue simplement une musique
///
/// Type : PlayMusic
/// Param�tre 1 : Fichier
/// Param�tre 2 : Bouclage ( facultatif )
/// Param�tre 3 : Volume ( facultatif )
////////////////////////////////////////////////////////////
bool ActPlayMusic( RuntimeScene & scene, ObjectsConcerned & objectsConcerned, const Instruction & action )
{
    SoundManager * soundManager = SoundManager::GetInstance();
    RessourcesLoader * ressourcesLoader = RessourcesLoader::GetInstance();

    Music * music = ressourcesLoader->LoadMusic(action.GetParameter(0).GetPlainString()); //Chargement

    soundManager->musics.push_back(music); //Ajout aux soundManager qui prend en charge la musique
    soundManager->musics.back()->Play();

    //Compatibilit� avec Game Develop 1.0.1979 et inf�rieur
    //On verifie si l'argument 2 ( Bouclage ) existe
    if ( action.GetParameters().size() > 1 )
    {
        music->SetLoop(action.GetParameter(1).GetAsBool());
    }

    //Compatibilit� avec Game Develop 1.1.5429 et inf�rieur
    //On verifie si l'argument 3 ( Volume ) existe
    if ( action.GetParameters().size() > 2 )
    {
        if ( !action.GetParameter(2).GetPlainString().empty() )
            music->SetVolume(action.GetParameter( 2 ).GetAsMathExpressionResult(scene, objectsConcerned));
        else
            music->SetVolume(100);
    }

    //Compatibility with Game Develop 1.5.9980 and below
    if ( action.GetParameters().size() > 3 )
    {
        if ( !action.GetParameter(3).GetPlainString().empty() )
            music->SetPitch(action.GetParameter( 3 ).GetAsMathExpressionResult(scene, objectsConcerned));
        else
            music->SetPitch(1);
    }

    return true;
}


////////////////////////////////////////////////////////////
/// Joue une musique sur un canal
///
/// Type : PlayMusicCanal
/// Param�tre 1 : Fichier
/// Param�tre 2 : Canal
/// Param�tre 3 : Bouclage ( facultatif )
////////////////////////////////////////////////////////////
bool ActPlayMusicCanal( RuntimeScene & scene, ObjectsConcerned & objectsConcerned, const Instruction & action )
{
    int canal = static_cast<int> ( action.GetParameter( 1 ).GetAsMathExpressionResult(scene, objectsConcerned));
    if ( canal < 0 || canal > MAX_CANAUX_MUSIC ) return false;

    SoundManager * soundManager = SoundManager::GetInstance();
    RessourcesLoader * ressourcesLoader = RessourcesLoader::GetInstance();

    Music * music = ressourcesLoader->LoadMusic(action.GetParameter(0).GetPlainString()); //Chargement
    music->Play();

    soundManager->SetMusicOnChannel(canal, music); //Ajout au soundManager qui prend en charge la music

    //Compatibilit� avec Game Develop 1.0.1979 et inf�rieur
    //On verifie si l'argument 3 ( Bouclage ) existe
    if ( action.GetParameters().size() > 2 )
    {
        music->SetLoop(action.GetParameter(2).GetAsBool());
    }

    //Compatibilit� avec Game Develop 1.1.5429 et inf�rieur
    //On verifie si l'argument 4 ( Volume ) existe
    if ( action.GetParameters().size() > 3 )
    {
        if ( !action.GetParameter(3).GetPlainString().empty() )
            music->SetVolume(action.GetParameter( 3 ).GetAsMathExpressionResult(scene, objectsConcerned));
        else
            music->SetVolume(100);
    }

    //Compatibility with Game Develop 1.5.9980 and below
    if ( action.GetParameters().size() > 4 )
    {
        if ( !action.GetParameter(4).GetPlainString().empty() )
            music->SetPitch(action.GetParameter( 4 ).GetAsMathExpressionResult(scene, objectsConcerned));
        else
            music->SetPitch(1);
    }

    return true;
}

////////////////////////////////////////////////////////////
/// Stoppe une musique sur un canal
///
/// Type : StopMusicCanal
/// Param�tre 1 : Canal
////////////////////////////////////////////////////////////
bool ActStopMusicCanal( RuntimeScene & scene, ObjectsConcerned & objectsConcerned, const Instruction & action )
{
    int canal = static_cast<int> ( action.GetParameter( 0 ).GetAsMathExpressionResult(scene, objectsConcerned));
    if ( canal < 0 || canal > MAX_CANAUX_MUSIC ) return false;

    SoundManager::GetInstance()->GetMusicOnChannel(canal)->Stop();

    return true;
}

////////////////////////////////////////////////////////////
/// Met en pause une musique sur un canal
///
/// Type : PauseMusicCanal
/// Param�tre 1 : Canal
////////////////////////////////////////////////////////////
bool ActPauseMusicCanal( RuntimeScene & scene, ObjectsConcerned & objectsConcerned, const Instruction & action )
{
    int canal = static_cast<int> ( action.GetParameter( 0 ).GetAsMathExpressionResult(scene, objectsConcerned));
    if ( canal < 0 || canal > MAX_CANAUX_MUSIC ) return false;

    SoundManager::GetInstance()->GetMusicOnChannel(canal)->Pause();

    return true;
}

////////////////////////////////////////////////////////////
/// Re(joute) une musique sur un canal
///
/// Type : RePlayMusicCanal
/// Param�tre 1 : Canal
////////////////////////////////////////////////////////////
bool ActRePlayMusicCanal( RuntimeScene & scene, ObjectsConcerned & objectsConcerned, const Instruction & action )
{
    int canal = static_cast<int> ( action.GetParameter( 0 ).GetAsMathExpressionResult(scene, objectsConcerned));
    if ( canal < 0 || canal > MAX_CANAUX_MUSIC )  return false;

    SoundManager::GetInstance()->GetMusicOnChannel(canal)->Play();

    return true;
}

////////////////////////////////////////////////////////////
/// Modifier le volume du son sur le canal
///
/// Type : ModVolumeSoundCanal
/// Param�tre 1 : Canal
/// Param�tre 2 : Volume
/// Param�tre 3 : Signe ( facultatif )
////////////////////////////////////////////////////////////
bool ActModVolumeSoundCanal( RuntimeScene & scene, ObjectsConcerned & objectsConcerned, const Instruction & action )
{
    int canal = static_cast<int> (action.GetParameter( 0 ).GetAsMathExpressionResult(scene, objectsConcerned));
    if ( canal < 0 || canal > MAX_CANAUX_SON ) return false;

    SoundManager * soundManager = SoundManager::GetInstance();

    if (action.GetParameter(2).GetPlainString().empty() || action.GetParameter(2).GetAsModOperator() == GDExpression::Set)
        soundManager->GetSoundOnChannel(canal)->SetVolume(action.GetParameter( 1 ).GetAsMathExpressionResult(scene, objectsConcerned));
    else if (action.GetParameter(2).GetAsModOperator() == GDExpression::Add)
        soundManager->GetSoundOnChannel(canal)->SetVolume(soundManager->GetSoundOnChannel(canal)->GetVolume() + action.GetParameter( 1 ).GetAsMathExpressionResult(scene, objectsConcerned));
    else if (action.GetParameter(2).GetAsModOperator() == GDExpression::Substract)
        soundManager->GetSoundOnChannel(canal)->SetVolume(soundManager->GetSoundOnChannel(canal)->GetVolume() - action.GetParameter( 1 ).GetAsMathExpressionResult(scene, objectsConcerned));
    else if (action.GetParameter(2).GetAsModOperator() == GDExpression::Multiply)
        soundManager->GetSoundOnChannel(canal)->SetVolume(soundManager->GetSoundOnChannel(canal)->GetVolume() * action.GetParameter( 1 ).GetAsMathExpressionResult(scene, objectsConcerned));
    else if (action.GetParameter(2).GetAsModOperator() == GDExpression::Divide)
        soundManager->GetSoundOnChannel(canal)->SetVolume(soundManager->GetSoundOnChannel(canal)->GetVolume() / action.GetParameter( 1 ).GetAsMathExpressionResult(scene, objectsConcerned));

    return true;
}


////////////////////////////////////////////////////////////
/// Modifier le volume d'une musique sur le canal
///
/// Type : ModVolumeMusicCanal
/// Param�tre 1 : Canal
/// Param�tre 2 : Volume
/// Param�tre 3 : Signe ( facultatif )
////////////////////////////////////////////////////////////
bool ActModVolumeMusicCanal( RuntimeScene & scene, ObjectsConcerned & objectsConcerned, const Instruction & action )
{
    int canal = static_cast<int> (action.GetParameter( 0 ).GetAsMathExpressionResult(scene, objectsConcerned));
    if ( canal < 0 || canal > MAX_CANAUX_MUSIC )  return false;

    SoundManager * soundManager = SoundManager::GetInstance();

    if (action.GetParameter(2).GetPlainString().empty() || action.GetParameter(2).GetAsModOperator() == GDExpression::Set)
        soundManager->GetMusicOnChannel(canal)->SetVolume(action.GetParameter( 1 ).GetAsMathExpressionResult(scene, objectsConcerned));
    else if (action.GetParameter(2).GetAsModOperator() == GDExpression::Add)
        soundManager->GetMusicOnChannel(canal)->SetVolume(soundManager->GetMusicOnChannel(canal)->GetVolume() + action.GetParameter( 1 ).GetAsMathExpressionResult(scene, objectsConcerned));
    else if (action.GetParameter(2).GetAsModOperator() == GDExpression::Substract)
        soundManager->GetMusicOnChannel(canal)->SetVolume(soundManager->GetMusicOnChannel(canal)->GetVolume() - action.GetParameter( 1 ).GetAsMathExpressionResult(scene, objectsConcerned));
    else if (action.GetParameter(2).GetAsModOperator() == GDExpression::Multiply)
        soundManager->GetMusicOnChannel(canal)->SetVolume(soundManager->GetMusicOnChannel(canal)->GetVolume() * action.GetParameter( 1 ).GetAsMathExpressionResult(scene, objectsConcerned));
    else if (action.GetParameter(2).GetAsModOperator() == GDExpression::Divide)
        soundManager->GetMusicOnChannel(canal)->SetVolume(soundManager->GetMusicOnChannel(canal)->GetVolume() / action.GetParameter( 1 ).GetAsMathExpressionResult(scene, objectsConcerned));

    return true;
}

////////////////////////////////////////////////////////////
/// Modifier le volume global du jeu
///
/// Type : ModGlobalVolume
/// Param�tre 2 : Volume
/// Param�tre 3 : Signe ( facultatif )
////////////////////////////////////////////////////////////
bool ActModGlobalVolume( RuntimeScene & scene, ObjectsConcerned & objectsConcerned, const Instruction & action )
{
    SoundManager * soundManager = SoundManager::GetInstance();

    if (action.GetParameter(1).GetPlainString().empty() || action.GetParameter(1).GetAsModOperator() == GDExpression::Set)
        soundManager->SetGlobalVolume(action.GetParameter( 0 ).GetAsMathExpressionResult(scene, objectsConcerned));
    else if (action.GetParameter(1).GetAsModOperator() == GDExpression::Add)
        soundManager->SetGlobalVolume(soundManager->GetGlobalVolume() + action.GetParameter( 0 ).GetAsMathExpressionResult(scene, objectsConcerned));
    else if (action.GetParameter(1).GetAsModOperator() == GDExpression::Substract)
        soundManager->SetGlobalVolume(soundManager->GetGlobalVolume() - action.GetParameter( 0 ).GetAsMathExpressionResult(scene, objectsConcerned));
    else if (action.GetParameter(1).GetAsModOperator() == GDExpression::Multiply)
        soundManager->SetGlobalVolume(soundManager->GetGlobalVolume() * action.GetParameter( 0 ).GetAsMathExpressionResult(scene, objectsConcerned));
    else if (action.GetParameter(1).GetAsModOperator() == GDExpression::Divide)
        soundManager->SetGlobalVolume(soundManager->GetGlobalVolume() / action.GetParameter( 0 ).GetAsMathExpressionResult(scene, objectsConcerned));

    return true;
}


void GD_API SetSoundPitchOnChannel( RuntimeScene & scene, unsigned int channel, float pitch )
{
    if ( channel > MAX_CANAUX_SON ) return;

    soundManager->GetSoundOnChannel(channel)->SetPitch(playingOffset);
}

void GD_API SetMusicPitchOnChannel( RuntimeScene & scene, unsigned int channel, float playingOffset )
{
    if ( channel > MAX_CANAUX_SON ) return;

    soundManager->GetMusicOnChannel(channel)->SetPitch(playingOffset);
}

double GD_API GetSoundPitchOnChannel( RuntimeScene & scene, unsigned int channel)
{
    if ( channel > MAX_CANAUX_SON ) return;

    return soundManager->GetSoundOnChannel(channel)->GetPitch(playingOffset);
}

double GD_API GetMusicPitchOnChannel( RuntimeScene & scene, unsigned int channel)
{
    if ( channel > MAX_CANAUX_SON ) return;

    return soundManager->GetMusicOnChannel(channel)->GetPitch(playingOffset);
}

void GD_API SetSoundPlayingOffsetOnChannel( RuntimeScene & scene, unsigned int channel, float playingOffset )
{
    if ( channel > MAX_CANAUX_SON ) return;

    soundManager->GetSoundOnChannel(channel)->SetPlayingOffset(playingOffset);
}

void GD_API SetMusicPlayingOffsetOnChannel( RuntimeScene & scene, unsigned int channel, float playingOffset )
{
    if ( channel > MAX_CANAUX_SON ) return;

    soundManager->GetMusicOnChannel(channel)->SetPlayingOffset(playingOffset);
}

double GD_API GetSoundPlayingOffsetOnChannel( RuntimeScene & scene, unsigned int channel)
{
    if ( channel > MAX_CANAUX_SON ) return;

    return soundManager->GetSoundOnChannel(channel)->GetPlayingOffset(playingOffset);
}

double GD_API GetMusicPlayingOffsetOnChannel( RuntimeScene & scene, unsigned int channel)
{
    if ( channel > MAX_CANAUX_SON ) return;

    return soundManager->GetMusicOnChannel(channel)->GetPlayingOffset(playingOffset);
}
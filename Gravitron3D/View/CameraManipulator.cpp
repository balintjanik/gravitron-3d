#include "CameraManipulator.h"

#include "Camera.h"

#include <SDL2/SDL.h>
#include <utility>

CameraManipulator::CameraManipulator()
{
}

CameraManipulator::~CameraManipulator()
{
}

void CameraManipulator::SetCamera( Camera* _pCamera )
{
    m_pCamera = _pCamera;

    if ( !m_pCamera ) return;

    // Set the initial spherical coordinates.
    m_center = m_pCamera->GetAt();
    glm::vec3 ToAim = m_center - m_pCamera->GetEye();

    m_distance = glm::length( ToAim );

    m_u = atan2f( ToAim.z, ToAim.x );
    m_v = acosf( ToAim.y / m_distance );

}

void CameraManipulator::Update( float _deltaTime )
{
    if ( !m_pCamera ) return;

    // Frissitjuk a kamerát a Model paraméterek alapján.

	// Az új nézési irányt a gömbi koordináták alapján számoljuk ki.
    glm::vec3 lookDirection( cosf(m_u) * sinf(m_v),
                             cosf(m_v), 
                             sinf(m_u) * sinf(m_v) );
	// Az új kamera pozíciót a nézési irány és a távolság alapján számoljuk ki.
    glm::vec3 eye = m_center - m_distance * lookDirection;

	// Az új felfelé irány a világ felfelével legyen azonos.
    glm::vec3 up = m_pCamera->GetWorldUp();

	// Az új jobbra irányt a nézési irány és a felfelé irány keresztszorzatából számoljuk ki.
    glm::vec3 right = glm::normalize( glm::cross( lookDirection, up ) );

	// Az új előre irányt a felfelé és jobbra irányok keresztszorzatából számoljuk ki.
    glm::vec3 forward = glm::cross( up, right);

	// Az új elmozdulásat a kamera mozgás irányának és sebességének a segítségével számoljuk ki.
    glm::vec3 deltaPosition = ( m_goForward * forward + m_goRight * right + m_goUp * up ) * m_speed * (10.0f * m_distance / MAX_DISTANCE) * _deltaTime;

	// Az új kamera pozíciót és nézési cél pozíciót beállítjuk.
    eye += deltaPosition;
    m_center += deltaPosition;   

	// Frissítjük a kamerát az új pozícióval és nézési iránnyal.
    m_pCamera->SetView( eye, m_center, m_pCamera->GetWorldUp() );
}


void CameraManipulator::KeyboardDown(const SDL_KeyboardEvent& key)
{
	switch ( key.keysym.sym )
	{
	case SDLK_LSHIFT:
	case SDLK_RSHIFT:
		if ( key.repeat == 0 ) m_speed /= 4.0f;
		break;
	case SDLK_w:
		m_goForward = 1;
		break;
	case SDLK_s:
		m_goForward = -1;
		break;
	case SDLK_a:
		m_goRight = -1;
		break;
	case SDLK_d:
		m_goRight = 1;
		break;
	case SDLK_e:
		m_goUp = 1;
		break;
	case SDLK_q:
		m_goUp = -1;
		break;
	}
}

void CameraManipulator::KeyboardUp(const SDL_KeyboardEvent& key)
{
	
	switch ( key.keysym.sym )
	{
	case SDLK_LSHIFT:
	case SDLK_RSHIFT:
		m_speed *= 4.0f;
		break;
	case SDLK_w:
	case SDLK_s:
		m_goForward = 0;
		break;
	case SDLK_a:
	case SDLK_d:
		m_goRight = 0;
		break;
	case SDLK_q:
	case SDLK_e:
		m_goUp = 0;
		break;
	}
}


void CameraManipulator::MouseMove(const SDL_MouseMotionEvent& mouse)
{
	if ( mouse.state & SDL_BUTTON_LMASK )
	{
		float du = mouse.xrel / 100.0f;
		float dv = mouse.yrel / 100.0f;

		m_u += du;
		m_v = glm::clamp<float>( m_v + dv, 0.1f, 3.1f );
	}
	if ( mouse.state & SDL_BUTTON_RMASK )
	{
		float dDistance = mouse.yrel / 100.0f;
		m_distance += dDistance;
	}
	if (mouse.state & SDL_BUTTON_MMASK)
	{
		glm::vec3 up = m_pCamera->GetWorldUp();
		glm::vec3 lookDirection(
			cosf(m_u) * sinf(m_v),
			cosf(m_v),
			sinf(m_u) * sinf(m_v));

		glm::vec3 right = glm::normalize(glm::cross(lookDirection, up));
		glm::vec3 forward = glm::cross(up, right);

		float panSpeed = m_distance * 0.002f;

		glm::vec3 moveRight = -right * static_cast<float>(mouse.xrel);
		glm::vec3 moveUp = forward * static_cast<float>(mouse.yrel);

		glm::vec3 panOffset = (moveRight + moveUp) * panSpeed;

		m_center += panOffset;
	}
}

void CameraManipulator::MouseWheel(const SDL_MouseWheelEvent& wheel)
{
	float zoomFactor = std::max(m_distance * 0.3f, 0.05f);

	float dDistance = static_cast<float>(wheel.y) * m_speed * zoomFactor / -100.0f;
	m_distance += dDistance;
	if (m_distance <= MIN_DISTANCE)
		m_distance = MIN_DISTANCE;
	else if (m_distance >= MAX_DISTANCE)
		m_distance = MAX_DISTANCE;
}
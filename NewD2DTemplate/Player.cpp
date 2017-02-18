#include "Player.h"
#include "Graphics.h"
Player::Player(b2World * world, Vec2f pos, D2D1_SIZE_F& dim)
	:m_dimensions(dim)
{
	m_indexes.push_back(m_leftIndices);
	m_indexes.push_back(m_rightIndices);
	m_indexes.push_back(m_rightJumpIndices);
	m_indexes.push_back(m_leftJumpIndices);
	b2BodyDef bDef;
	bDef.type = b2_dynamicBody;
	bDef.awake = true;
	bDef.fixedRotation = true;
	bDef.position.Set(pos.x, pos.y);

	m_body = world->CreateBody(&bDef);
	b2PolygonShape shape;

	shape.SetAsBox(m_dimensions.width / 2, m_dimensions.height / 2);
	b2FixtureDef fix;
	fix.shape = &shape;
	fix.density = 1.01f;
	fix.friction = 8.0f;
	
	m_fixture = m_body->CreateFixture(&fix);
	m_body->SetGravityScale(5.0f);
}

void Player::Update(float dt, Keyboard& kbd)
{
	if (kbd.KeyIsPressed(VK_RIGHT)&& !m_Dir.jumping)
	{
		m_indicesIndex = 1;
		isMoving = true;
		m_Dir.right = true;
		m_Dir.left = false;
		//m_body->ApplyLinearImpulseToCenter(b2Vec2(10.0f, 0.0f), true);
		m_body->ApplyForceToCenter(b2Vec2(1000000.0f, 0.0f), true);
	}
	else if (kbd.KeyIsPressed(VK_LEFT) && !m_Dir.jumping)
	{
		m_indicesIndex = 0;
		isMoving = true;
		m_Dir.right = false;
		m_Dir.left = true;
		m_body->ApplyForceToCenter(b2Vec2(-1000000.0f, 0.0f), true);
	}
	else
	{
		if (m_indicesIndex == 0)
			m_incIndex = 1;
		else if (m_indicesIndex == 1)
			m_incIndex = 2;
		isMoving = false;
	}
	if (isMoving)
	{
		if ((timer += dt) > timerInc)
		{
			timer = 0;
			m_incIndex++;
			if (m_incIndex > 3)
				m_incIndex = 0;
		}
	}else
	{
		timer = 0.0f;
	}
	if (m_body->GetLinearVelocity().y > 0.0f && !isMoving)
	{
		if (m_Dir.left)
		{
			m_indicesIndex = 3;
			m_incIndex = 1;
		}
		if (m_Dir.right)
		{
			m_indicesIndex = 2;
			m_incIndex = 3;
		}
		m_Dir.jumping = true;
	}
	for (b2ContactEdge* ce = m_body->GetContactList(); ce != nullptr; ce = ce->next)
	{
		b2Contact* contact = ce->contact;
		if (contact->IsTouching())
		{
			b2WorldManifold man;
			contact->GetWorldManifold(&man);
			bool below = false;
			for (int i = 0; i < b2_maxManifoldPoints; i++)
			{
				if (man.points[i].y > m_body->GetPosition().y + m_dimensions.height / 2.0f)
				{
					
					if (m_Dir.left && m_Dir.jumping)
					{
						m_indicesIndex = 0;
						m_incIndex = 1;
					}
					if (m_Dir.right&& m_Dir.jumping)
					{
						m_indicesIndex = 1;
						m_incIndex = 2;
					}
					m_Dir.jumping = false;
					below = true;
					break;
				}
			}
			if (below)
			{
				if (kbd.KeyIsPressed(VK_UP))
				{
					if (m_Dir.left)
					{
						m_indicesIndex = 3;
						m_incIndex = 1;
					}
					if (m_Dir.right)
					{
						m_indicesIndex = 2;
						m_incIndex = 3;
					}
					m_Dir.jumping = true;
					isMoving = false;
					m_body->ApplyLinearImpulse(b2Vec2(0.0f, -50000.0f), b2Vec2(0.0f, 0.0f), true);
				}
			}
		}
	}
	TextureManager::ImageClip clip;
	clip = Locator::ImageManager->GetClip("dude", m_indexes[m_indicesIndex][m_incIndex]);
	m_image = clip.bitmap;
	m_clipRect = clip.rect.ToD2D();
}
Player::Drawable Player::GetDrawable()
{
	return Drawable(*this);
}
b2Vec2 Player::GetPosition()
{
	return m_body->GetPosition();
}
void Player::Drawable::Rasterize(Graphics& gfx)
{
	matRot = D2D1::Matrix3x2F::Rotation(Utils::RadToDeg(m_parent.m_body->GetAngle()),
	{ m_parent.m_body->GetPosition().x,m_parent.m_body->GetPosition().y });

	D2D1_RECT_F rect = D2D1::RectF(
		m_parent.m_body->GetPosition().x - (m_parent.m_dimensions.width / 2.0f)-24.0f,
		m_parent.m_body->GetPosition().y - (m_parent.m_dimensions.height / 2.0f),
		m_parent.m_body->GetPosition().x + (m_parent.m_dimensions.width / 2.0f) +24.0f,
		m_parent.m_body->GetPosition().y + (m_parent.m_dimensions.height / 2.0f) );
	gfx.DrawPlayer(matRot*matTrans, Player::RenderDesc(m_parent.m_image, rect, m_parent.m_clipRect, m_parent.m_transparency));

}
void Player::Drawable::Transform(const D2D1::Matrix3x2F& mat)
{
	matTrans = mat;
}

RectF Player::Drawable::GetAABB()
{
	return RectF(
		m_parent.m_body->GetPosition().x - m_parent.m_dimensions.width * 0.5f,
		m_parent.m_body->GetPosition().y - m_parent.m_dimensions.height * 0.5f,
		m_parent.m_body->GetPosition().x + m_parent.m_dimensions.width * 0.5f,
		m_parent.m_body->GetPosition().y + m_parent.m_dimensions.height * 0.5f
	);
}

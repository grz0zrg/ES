/*
* Copyright (c) 2006, 2007, 2008 Julien Verneuil
* Julien Verneuil a.k.a Grz-
* All rights reserved.
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of ES or ES Engine nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// Les 2 fonctions de collisions ont étaît prises sur http://fearyourself.developpez.com/tutoriel/sdl/pong/partie-3-collisions-menu/#L2.3
// La license contenu sur la page m'autorise à utiliser ses fonctions dans mon moteur cf. citation :
// "Les sources présentées sur cette page sont libres de droits, et vous pouvez les utiliser à votre convenance."

#include "ES_Physics/physics.h"

namespace ES
{
	bool ES_Physics::circleCollision(float cx1, float cy1, float r1, float cx2, float cy2, float r2)
	{
	   d = (cx1 - cx2) * (cx1 - cx2) + (cy1 - cy2) * (cy1 - cy2);
	   d = sqrt(d);

	   return (d < r1 + r2);
	}

	bool ES_Physics::rectCollision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2)
	{
		if(x1 + w1 < x2)
			return false;
		if(x2 + w2 < x1)
			return false;
		if(y1 + h1 < y2)
			return false;
		if(y2 + h2 < y1)
			return false;

		return true;
	}

    void ES_Physics::setupWorld(float min_x, float min_y, float max_x, float max_y, float gravx, float gravy, bool dosleep)
    {
        b2AABB worldAABB;
        worldAABB.lowerBound.Set(min_x, min_y);
        worldAABB.upperBound.Set(max_x, max_y);

        b2Vec2 gravity(gravx, gravy);

        world = new b2World(worldAABB, gravity, dosleep);
    }

    void ES_Physics::addGround(float size_x, float size_y, float pos_x, float pos_y)
    {
        //b2BoxDef groundBoxDef;
        //groundBoxDef.extents.Set(size_x, size_y);
        //groundBoxDef.density = 0.0f;

        b2BodyDef groundBodyDef;
        groundBodyDef.position.Set(pos_x, pos_y);
        //groundBodyDef.AddShape(&groundBoxDef);

        b2Vec2 vec(size_x, size_x);

        ground_list.push_back(world->CreateBody(&groundBodyDef));

        b2PolygonDef groundShapeDef;
        groundShapeDef.SetAsBox(size_x, size_y);

        if(!ground_list.empty())
            ground_list[ground_list.size()-1]->CreateShape(&groundShapeDef);

        ground_list_size.push_back(vec);
    }

    void ES_Physics::addBox(float pos_x, float pos_y, float size_x, float size_y, float density, float angle, float friction, float linear_vel_x, float linear_vel_y, bool sleep, bool enable_rot)
    {/*
        b2BoxDef sd;
        sd.type = e_boxShape;
        sd.extents.Set(size_x, size_y);
        sd.density = density;
*/

        b2BodyDef bd;
        bd.allowSleep = sleep;
        bd.position.Set(pos_x, pos_y);
        bd.fixedRotation = enable_rot;
        bd.angle = angle;

        b2Vec2 vel(linear_vel_x, linear_vel_y);

        box_list.push_back(world->CreateBody(&bd));

        b2PolygonDef shapeDef;
        shapeDef.SetAsBox(size_x, size_y);
        shapeDef.density = density;
        shapeDef.friction = friction;

        if(!box_list.empty())
        {
            box_list[box_list.size()-1]->CreateShape(&shapeDef);
            box_list[box_list.size()-1]->SetMassFromShapes();
            box_list[box_list.size()-1]->SetLinearVelocity(vel);
        }

        vel.x = size_x;
        vel.y = size_y;

        box_list_size.push_back(vel);
    }

    void ES_Physics::updateWorld(float time_step, int iterations)
    {
        world->Step(time_step, iterations);
    }

    b2Vec2 ES_Physics::getPosition(unsigned int i, unsigned short int type)
    {
        if(!box_list.empty() && !ground_list.empty())
        {
            if(type == ES_BOX && i < box_list.size())
                return box_list[i]->GetPosition();
            else if(type == ES_GROUND && i < ground_list.size())
                return ground_list[i]->GetPosition();
        }
        b2Vec2 ret(0, 0);
        return ret;
    }

    float ES_Physics::getRotation(unsigned int i, unsigned short int type)
    {
        if(!box_list.empty() && !ground_list.empty())
        {
            if(type == ES_BOX && i < box_list.size())
                return box_list[i]->GetAngle();
            else if(type == ES_GROUND && i < ground_list.size())
                return ground_list[i]->GetAngle();
        }
        return 0;
    }

    void ES_Physics::applyForce(unsigned int i, unsigned short int type, float x, float y, float f_x, float f_y)
    {
        b2Vec2 force(f_x, f_y);
        b2Vec2 position(x, y);
        if(type == ES_BOX && i <= box_list.size())
            box_list[i]->ApplyForce(force,position);
        else if(type == ES_GROUND && i <= ground_list.size())
            ground_list[i]->ApplyForce(force,position);
    }

    int ES_Physics::countObject(unsigned short int type)
    {
        if(type == ES_BOX)
            return box_list.size();
        else if(type == ES_GROUND)
            return ground_list.size();
        return -1;
    }

    void ES_Physics::delObject(unsigned int i, unsigned short int type)
    {
        if(type == ES_BOX && i < box_list.size())
        {
            world->DestroyBody(box_list[i]);
            box_list_size.erase(box_list_size.begin()+i);
            box_list.erase(box_list.begin()+i);
        }
        else if(type == ES_GROUND && i < ground_list.size())
        {
            world->DestroyBody(ground_list[i]);
            ground_list_size.erase(ground_list_size.begin()+i);
            ground_list.erase(ground_list.begin()+i);
        }
    }

    void ES_Physics::freeWorld()
    {
        for(unsigned int i = 0; i < box_list.size(); i++)
            world->DestroyBody(box_list[i]);
        for(unsigned int i = 0; i < ground_list.size(); i++)
            world->DestroyBody(ground_list[i]);
        box_list.clear();
        ground_list.clear();
        box_list_size.clear();
        ground_list_size.clear();
        delete world;
    }
}

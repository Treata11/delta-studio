#include <pch.h>

#include "../include/delta_physics.h"
#include "utilities.h"

TEST(DeltaPhysicsTest, SanityCheck) {
    EXPECT_EQ(1, 1);
    EXPECT_TRUE(true);
}

TEST(CollisionTests, CircleCircleSanityCheck) {
    dphysics::CirclePrimitive c1;
    dphysics::CirclePrimitive c2;

    c1.Position = ysMath::LoadVector(1.0f, 0.0f, 0.0f, 0.0f);
    c1.Radius = 1.0f;

    c2.Position = ysMath::LoadVector(1.5f, 0.0f, 0.0f, 0.0f);
    c2.Radius = 1.0f;

    dphysics::CollisionDetector detector;
    dphysics::Collision collisions[2];
    int n = detector.CircleCircleCollision(collisions, nullptr, nullptr, &c1, &c2);

    EXPECT_TRUE(n == 1);
    EXPECT_NEAR(collisions[0].m_penetration, 1.5f, 1E-4);

    VecEq(collisions[0].m_position, ysMath::LoadVector(2.0f, 0.0f, 0.0f, 0.0f));
}

TEST(CollisionTests, CircleCircleNotColliding) {
    dphysics::CirclePrimitive c1;
    dphysics::CirclePrimitive c2;

    c1.Position = ysMath::LoadVector(1.0f, 0.0f, 0.0f, 0.0f);
    c1.Radius = 1.0f;

    c2.Position = ysMath::LoadVector(5.5f, 0.0f, 0.0f, 0.0f);
    c2.Radius = 1.0f;

    dphysics::CollisionDetector detector;
    dphysics::Collision collisions[2];
    int n = detector.CircleCircleCollision(collisions, nullptr, nullptr, &c1, &c2);

    EXPECT_TRUE(n == 0);
}

TEST(CollisionTests, BoxBoxSanityCheck) {
    dphysics::BoxPrimitive b1;
    dphysics::BoxPrimitive b2;

    b1.Position = ysMath::LoadVector(0.0f, 0.0f, 0.0f, 0.0f);
    b1.HalfHeight = 0.5f;
    b1.HalfWidth = 0.5f;
    b1.Orientation = ysMath::Constants::QuatIdentity;

    b2.Position = ysMath::LoadVector(1.0f - 0.001f, 0.0f, 0.0f, 0.0f);
    b2.HalfHeight = 0.5f;
    b2.HalfWidth = 0.5f;
    b2.Orientation = ysMath::Constants::QuatIdentity;

    dphysics::CollisionDetector detector;
    dphysics::Collision collisions[2];
    int n = detector.BoxBoxCollision(collisions, nullptr, nullptr, &b1, &b2);

    EXPECT_TRUE(n > 0);
    EXPECT_GE(collisions[0].m_penetration, 0);
    EXPECT_LE(collisions[0].m_penetration, 0.001f);
}

TEST(CollisionTests, BoxBoxOffCenter) {
    dphysics::BoxPrimitive b1;
    dphysics::BoxPrimitive b2;

    b1.Position = ysMath::LoadVector(0.0f, 0.0f, 0.0f, 0.0f);
    b1.HalfHeight = 0.5f;
    b1.HalfWidth = 0.5f;
    b1.Orientation = ysMath::Constants::QuatIdentity;

    b2.Position = ysMath::LoadVector(1.0f - 0.001f, 0.25f, 0.0f, 0.0f);
    b2.HalfHeight = 0.5f;
    b2.HalfWidth = 0.5f;
    b2.Orientation = ysMath::Constants::QuatIdentity;

    dphysics::CollisionDetector detector;
    dphysics::Collision collisions[2];
    int n = detector.BoxBoxCollision(collisions, nullptr, nullptr, &b1, &b2);

    EXPECT_TRUE(n > 0);
    EXPECT_GE(collisions[0].m_penetration, 0);
    EXPECT_LE(collisions[1].m_penetration, 0.001f);
}

TEST(CollisionTests, BoxBoxOffCenterDifferentSizes) {
    dphysics::BoxPrimitive b1;
    dphysics::BoxPrimitive b2;

    constexpr float Penetration = 0.001f;

    b1.Position = ysMath::LoadVector(0.0f, 0.0f, 0.0f, 0.0f);
    b1.HalfHeight = 0.5f;
    b1.HalfWidth = 0.5f;
    b1.Orientation = ysMath::Constants::QuatIdentity;

    b2.Position = ysMath::LoadVector(0.75f - Penetration, 0.25f, 0.0f, 0.0f);
    b2.HalfHeight = 0.25f;
    b2.HalfWidth = 0.25f;
    b2.Orientation = ysMath::Constants::QuatIdentity;

    dphysics::CollisionDetector detector;
    dphysics::Collision collisions[2];
    int n = detector.BoxBoxCollision(collisions, nullptr, nullptr, &b1, &b2);

    EXPECT_TRUE(n > 0);
    EXPECT_GE(collisions[0].m_penetration, 0);
    EXPECT_LE(collisions[1].m_penetration, Penetration);
}

TEST(CollisionTests, BoxBoxOffCenterDifferentSizesDeep) {
    dphysics::BoxPrimitive b1;
    dphysics::BoxPrimitive b2;

    constexpr float Penetration = 0.01f;

    b1.Position = ysMath::LoadVector(0.0f, 0.0f, 0.0f, 0.0f);
    b1.HalfHeight = 0.5f;
    b1.HalfWidth = 0.5f;
    b1.Orientation = ysMath::Constants::QuatIdentity;

    b2.Position = ysMath::LoadVector(0.75f - Penetration, 0.5f, 0.0f, 0.0f);
    b2.HalfHeight = 0.25f;
    b2.HalfWidth = 0.25f;
    b2.Orientation = ysMath::Constants::QuatIdentity;

    dphysics::RigidBody a;
    dphysics::RigidBody b;

    dphysics::CollisionDetector detector;
    dphysics::Collision collisions[2];
    int n = detector.BoxBoxCollision(collisions, &a, &b, &b1, &b2);

    EXPECT_TRUE(n > 0);
    EXPECT_GE(collisions[0].m_penetration, 0);
    EXPECT_LE(collisions[0].m_penetration, Penetration);
    EXPECT_EQ(collisions[0].m_body1, &b);
    EXPECT_EQ(collisions[0].m_body2, &a);
}

TEST(CollisionTests, BoxBoxOffCenterDifferentSizesDeep2) {
    dphysics::BoxPrimitive b1;
    dphysics::BoxPrimitive b2;

    b1.Position = ysMath::LoadVector(0.0f, 0.0f, 0.0f, 0.0f);
    b1.HalfHeight = 0.5f;
    b1.HalfWidth = 0.5f;
    b1.Orientation = ysMath::Constants::QuatIdentity;

    b2.Position = ysMath::LoadVector(1.682f, 1.08866f, 0.0f, 0.0f);
    b2.HalfHeight = 1.25f;
    b2.HalfWidth = 1.25f;
    b2.Orientation = ysMath::Constants::QuatIdentity;

    dphysics::RigidBody a;
    dphysics::RigidBody b;

    dphysics::CollisionDetector detector;
    dphysics::Collision collisions[2];
    int n = detector.BoxBoxCollision(collisions, &b, &a, &b2, &b1);

    EXPECT_TRUE(n > 0);
    EXPECT_GE(collisions[0].m_penetration, 0);
    EXPECT_EQ(collisions[0].m_body1, &a);
    EXPECT_EQ(collisions[0].m_body2, &b);
    VecEq(collisions[0].m_normal, ysMath::LoadVector(-1.0f, 0.0f, 0.0f));
}

TEST(CollisionTests, BoxBoxBigSmall) {
    dphysics::BoxPrimitive b1;
    dphysics::BoxPrimitive b2;

    b1.Position = ysMath::LoadVector(0.0f, 0.0f, 0.0f, 0.0f);
    b1.HalfHeight = 10.0f;
    b1.HalfWidth = 10.0f;
    b1.Orientation = ysMath::Constants::QuatIdentity;

    b2.Position = ysMath::LoadVector(11.4f, 0.0f, 0.0f, 0.0f);
    b2.HalfHeight = 1.5f;
    b2.HalfWidth = 1.5f;
    b2.Orientation = ysMath::Constants::QuatIdentity;

    dphysics::RigidBody a;
    dphysics::RigidBody b;

    dphysics::CollisionDetector detector;
    dphysics::Collision collisions[2];
    int n = detector.BoxBoxCollision(collisions, &b, &a, &b2, &b1);

    EXPECT_TRUE(n == 2);
    EXPECT_NEAR(collisions[0].m_penetration, 0.1f, 1E-4);
}

TEST(CollisionTests, RayCircleCollision) {
    dphysics::RayPrimitive b1;
    dphysics::CirclePrimitive b2;

    b1.Position = ysMath::LoadVector(0.0f, 0.0f, 0.0f, 0.0f);
    b1.Direction = ysMath::LoadVector(1.0f, 0.0f, 0.0f, 0.0f);

    b2.Position = ysMath::LoadVector(10.0f, 0.0f, 0.0f, 0.0f);
    b2.Radius = 1.0f;

    dphysics::RigidBody a;
    dphysics::RigidBody b;

    dphysics::CollisionDetector detector;
    dphysics::Collision collisions[2];
    int n = detector.RayCircleCollision(collisions, &b, &a, &b1, &b2);

    EXPECT_TRUE(n == 1);
    EXPECT_NEAR(collisions[0].m_penetration, 9.0f, 1E-4);

    b2.Position = ysMath::LoadVector(10.0f, 1.0f, 0.0f, 0.0f); 
    n = detector.RayCircleCollision(collisions, &b, &a, &b1, &b2);

    EXPECT_TRUE(n == 1);
    EXPECT_NEAR(collisions[0].m_penetration, 10.0f, 1E-4);

    b2.Position = ysMath::LoadVector(-10.0f, 1.0f, 0.0f, 0.0f);
    n = detector.RayCircleCollision(collisions, &b, &a, &b1, &b2);

    EXPECT_TRUE(n == 0);
}

TEST(CollisionTests, BoxCircleEdgeCase) {
    dphysics::BoxPrimitive b1;
    dphysics::CirclePrimitive b2;

    b1.Position = ysMath::LoadVector(0.1f, 0.0f, 0.0f, 1.0f);
    b1.HalfHeight = 1.0f;
    b1.HalfWidth = 1.0f;
    b1.Orientation = ysMath::Constants::QuatIdentity;

    b2.Position = ysMath::LoadVector(0.0f, 0.0f, 0.0f, 1.0f);
    b2.Radius = 1.0f;

    dphysics::RigidBody a;
    dphysics::RigidBody b;

    dphysics::CollisionDetector detector;
    dphysics::Collision collisions[2];
    int colliding = detector.CircleBoxCollision(collisions, &b, &a, &b2, &b1);

    EXPECT_TRUE(colliding == 1);
}

TEST(CollisionTests, BoxBoxSideGlitch) {
    dphysics::BoxPrimitive b1;
    dphysics::BoxPrimitive b2;

    b1.Position = ysMath::LoadVector(0.0f, 0.0f, 0.0f, 0.0f);
    b1.HalfHeight = 3.0f;
    b1.HalfWidth = 1.0f;
    b1.Orientation = ysMath::Constants::QuatIdentity;

    b2.Position = ysMath::LoadVector(1.0f + 0.5f - 0.1f, 1.0f, 0.0f, 0.0f);
    b2.HalfHeight = 1.5f;
    b2.HalfWidth = 0.5f;
    b2.Orientation = ysMath::Constants::QuatIdentity;

    dphysics::RigidBody a;
    dphysics::RigidBody b;

    dphysics::CollisionDetector detector;
    dphysics::Collision collisions[2];
    int n = detector.BoxBoxCollision(collisions, &b, &a, &b2, &b1);

    EXPECT_TRUE(n == 2);
    EXPECT_NEAR(collisions[0].m_penetration, 0.1f, 1E-4);
}

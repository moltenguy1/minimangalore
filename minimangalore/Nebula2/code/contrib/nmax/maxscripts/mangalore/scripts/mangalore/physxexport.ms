-------------------------------------------------------------------------------
--  physxexport.ms
--
--  (c)2007 Cho, Jun Hyeong
-------------------------------------------------------------------------------

include "mangalore/physxxmlutil.ms"


-- ODE Body �� collecting �ϱ� ���� ����ü
struct ODEBody ( o, m );


-------------------------------------------------------------------------------
-- @name nPoint3
--
-- nebula ��ǥ��� ��ȯ�Ѵ�.
-------------------------------------------------------------------------------
fn nPoint3 p =
(
    local str = ""
    str += ((-p.x) as string)
    str += " "
    str += (p.z as string)
    str += " "
    str += (p.y as string)
    return str;
)

-------------------------------------------------------------------------------
-- @name nQuat
--
-- nebula ��ǥ��� ��ȯ�Ѵ�.
-------------------------------------------------------------------------------
fn nQuat q =
(
    local str = ""
    str += ((-q.x) as string)
    str += " "
    str += (q.z as string)
    str += " "
    str += (q.y as string)
    str += " "
    str += ((-q.w) as string)
    return str;
)

-------------------------------------------------------------------------------

fn nSize3 s =
(
    local str = ""
    str += (s.width as string)
    str += " "
    str += (s.height as string)
    str += " "
    str += (s.length as string)
    return str;
)

-------------------------------------------------------------------------------
-- @name ExportODEBox
--
-- x = o.width
-- y = o.height
-- z = o.length
-------------------------------------------------------------------------------
fn ExportODEBox o =
(
--    o.pos.z += o.size.z * 1.5;
    pos = in coordsys parent o.pos;
    rot = in coordsys parent o.rotation;
    local size = ""
    size += (o.delegate.width as string);
    size += " "
    size += (o.delegate.height as string);
    size += " "
    size += (o.delegate.length as string);
	
    xmlElementBegin "BoxShape"
    xmlAddAttribute "pos" (nPoint3 pos)
    xmlAddAttribute "rot" (nQuat rot)
    xmlAddAttribute "size" size;
    xmlAddAttribute "mat" g_materials[o.materialIndex];
    xmlElementEnd() -- BoxShape

)

-------------------------------------------------------------------------------
-- @name ExportODESphere
--
-------------------------------------------------------------------------------
fn ExportODESphere o =
(
    pos = in coordsys parent o.pos;

    xmlElementBegin "SphereShape"
    xmlAddAttribute "pos" (nPoint3 pos)
    xmlAddAttribute "radius" o.delegate.radius;
    xmlAddAttribute "mat" g_materials[o.materialIndex];
    xmlElementEnd() -- SphereShape

)

-------------------------------------------------------------------------------
-- @name ExportODECapsule
--
-------------------------------------------------------------------------------
fn ExportODECapsule o =
(
    pos = in coordsys parent o.pos;
    rot = in coordsys parent o.rotation;

    xmlElementBegin "CapsuleShape"
    xmlAddAttribute "pos" (nPoint3 pos)
    xmlAddAttribute "rot" (nQuat rot)
    xmlAddAttribute "radius" o.delegate.radius;
    xmlAddAttribute "length" o.delegate.height;
    xmlAddAttribute "mat" g_materials[o.materialIndex];
    xmlElementEnd() -- CapsuleShape
)

-------------------------------------------------------------------------------
-- @name ExportODEShape
--
-------------------------------------------------------------------------------
fn ExportODEShape o =
(
    c = (classof o.baseobject);

    if( c == ODEBox ) then
    (
        ExportODEBox o;
    )
    else if( c == ODESphere ) then
    (
        ExportODESphere o;
    )
    else if( c == ODECapsule ) then
    (
        ExportODECapsule o;
    )
)

-------------------------------------------------------------------------------
-- @name IsODEShape
--
-------------------------------------------------------------------------------
fn IsODEShape o =
(
    c = (classof o);

    if( c == ODEBox ) then return true;
    else if( c == ODESphere ) then return true;
    else if( c == ODECapsule ) then return true;
    else return false;
)

-------------------------------------------------------------------------------
-- @name ExportODEBody
--
-- @desc
-- ODE Body(Rigid Body)�� ������ ȭ��� �������� ������Ʈ�� �����ؾ��Ѵ�.
-- ODE Body�� ������ ������Ʈ�� ODE Shape�� �ڽ����� ������ �ִ�.
--
-- @todo
-- rigidbody �� pos �� pivot ���� �ƴ϶� ���� �߽��� �ϱ�?
--
-- @tip
-- RigidBody class �� attach �Լ�����
-- RigidBody �� ������ Shape ���� POR(���� �߽���)�� ����ϹǷ�
-- RigidBody �� ��ġ�� ����� �������ϴ�.
--
-- �׷��� physics1.xml ��ó��
-- ode body �� joint ���� ���� �����ؼ� ��ũ�ϴ� ������ ����� ������ �ʾƵ� �ɵ��ϴ�.
-- �̷��� ����� �۾������ �� ���ŷ���� �ϴ�.
--
-- �ִ��� �߰����� helper�� dummy ������Ʈ�� �۾��� ���� data �� �����Ҽ� �ְ� �Ͽ�
-- �۾��� ���Ǽ��� �߱��Ѵ�.
--
-- ragdoll �� ����� rigidbody �� model�� �������� �ʴ´�.
-- biped �� ��ü�� �����Ѵ�.
-- skim mesh �� biped �� ���� �����̱� �����̴�.
-------------------------------------------------------------------------------
fn ExportODEBody o m isRagDoll =
(
    pos = in coordsys world o.pos;
    --pos = in coordsys world o.center;
    rot = in coordsys world o.rotation;

    xmlElementBegin "RigidBody"
    xmlAddAttribute "name" o.name;
    xmlAddAttribute "pos" (nPoint3 pos)
        xmlAddAttribute "rot" (nQuat rot)

    --if( m.hasJoint == true )
    --    xmlAddAttribute "jointCollide" "true";

    if( (classof o.baseobject) == Editable_poly ) then
    (
        xmlAddAttribute "model" o.name
    )

    --xmlAddAttribute "shadow" "?";

    -- if physic type is rag doll(2),
    if( isRagDoll ) then
    (
       xmlAddAttribute "joint" o.name
    )

    -- collect ode shape
    local shapeArray = #();
    for c in o.children do
    (
        if( IsODEShape(c) == true ) then
        (
            append shapeArray c;
        )
    )

    xmlAddAttribute "numShapes" shapeArray.count;

    -- export ode shape
    for shape in shapeArray do
    (
        ExportODEShape shape;
    )

    xmlElementEnd() -- RigidBody
)

-------------------------------------------------------------------------------
-- @name ExportODEJoint
--
-- object �� ode body modifier �� ������ �ǰ� joint �� ������ �Ǿ��� ���.
-- �� ����Ʈ�� ��ũ�� body1, body2 �� �ڽŰ� �ڽ��� �θ��̴�.
-- �� �ڽŰ� �ڽ��� �θ�� ode body �� �����Ǿ�� �Ѵ�.
-- �θ� ���� ��� ����ȯ�濡 ��ũ�ȴ�.
-------------------------------------------------------------------------------
fn ExportODEJoint o m =
(
    if( m.hasJoint == false ) then return;

    -- ball
    if( m.jointType == 2 ) then
    (
        ExportJointBallAndSocket o m
    )
    -- hinge
    else if( m.jointType == 3 ) then
    (
        ExportJointHinge o m
    )
    -- hinge2
    else if( m.jointType == 4 ) then
    (
        ExportJointHinge2 o m
    )
    -- slider
    else if( m.jointType == 5 ) then
    (
        ExportJointSlider o m
    )
    -- universal
    else if( m.jointType == 6 ) then
    (
        ExportJointUniversal o m
    )
    -- amotor
    else if( m.jointType == 7 ) then
    (
        ExportJointAMotor o m
    )
    -- fixed
    else if( m.jointType == 8 ) then
    (
        -- not implemented
    )
)

-------------------------------------------------------------------------------
-- @name ExportJointBallAndSocket
--
-------------------------------------------------------------------------------
fn ExportJointBallAndSocket o m =
(
    xmlElementBegin "BallAndSocket"
    xmlAddAttribute "body1" o.name

    p = o.parent
    for m in p.modifiers do
    (
        if( m.name == "ODE Body (Mangalore)" ) then
        (
            xmlAddAttribute "body2" p.name
        )
    )

    --xmlAddAttribute "joint" ?
    xmlAddAttribute "anchor" o.pos
    xmlElementEnd() -- BallAndSocket
)

-------------------------------------------------------------------------------
-- @name ExportJointHinge
--
-------------------------------------------------------------------------------
fn ExportJointHinge o m =
(
    xmlElementBegin "Hinge"
    xmlAddAttribute "body1" o.name

    p = o.parent
    for m in p.modifiers do
    (
        if( m.name == "ODE Body (Mangalore)" ) then
        (
            xmlAddAttribute "body2" p.name
        )
    )

    --xmlAddAttribute "joint" ?
    xmlAddAttribute "anchor" o.pos
    
    -- axis
    xmlAddAttribute "axis" o.pos        -- default: axis(0.0, 1.0, 0.0)
    if( m.isLoStop0 ) then
    (
        xmlAddAttribute "lostop0" m.loStop0
    )
    if( m.isHiStop0 ) then
    (
        xmlAddAttribute "histop0" m.hiStop0
    )
    xmlAddAttribute "vel0" m.vel0
    xmlAddAttribute "fmax0" m.fMax0
    xmlAddAttribute "fudge0" m.fudge0
    xmlAddAttribute "bounce0" m.bounce0
    xmlAddAttribute "cfm0" m.cfm0
    xmlAddAttribute "stopErp0" m.stopErp0
    xmlAddAttribute "stopCfm0" m.stopCfm0

    xmlElementEnd() -- Hinge
)

-------------------------------------------------------------------------------
-- @name ExportJointHinge2
--
-------------------------------------------------------------------------------
fn ExportJointHinge2 o m =
(
    xmlElementBegin "Hinge2"
    xmlAddAttribute "body1" o.name

    p = o.parent
    for m in p.modifiers do
    (
        if( m.name == "ODE Body (Mangalore)" ) then
        (
            xmlAddAttribute "body2" p.name
        )
    )

    --xmlAddAttribute "joint" ?
    xmlAddAttribute "anchor" o.pos

    -- axis #1
    xmlAddAttribute "axis1" o.pos        -- default: axis(0.0, 1.0, 0.0)
    if( m.isLoStop0 ) then
    (
        xmlAddAttribute "lostop0" m.loStop0
    )
    if( m.isHiStop0 ) then
    (
        xmlAddAttribute "histop0" m.hiStop0
    )
    xmlAddAttribute "vel0" m.vel0
    xmlAddAttribute "fmax0" m.fMax0
    xmlAddAttribute "fudge0" m.fudge0
    xmlAddAttribute "bounce0" m.bounce0
    xmlAddAttribute "cfm0" m.cfm0
    xmlAddAttribute "stopErp0" m.stopErp0
    xmlAddAttribute "stopCfm0" m.stopCfm0

    -- axis #2
    xmlAddAttribute "axis2" o.pos        -- default: axis(0.0, 1.0, 0.0)
    if( m.isLoStop1 ) then
    (
        xmlAddAttribute "lostop1" m.loStop1
    )
    if( m.isHiStop1 ) then 
    (
        xmlAddAttribute "histop1" m.hiStop1
    )
    xmlAddAttribute "vel1" m.vel1
    xmlAddAttribute "fmax1" m.fMax1
    xmlAddAttribute "fudge1" m.fudge1
    xmlAddAttribute "bounce1" m.bounce1
    xmlAddAttribute "cfm1" m.cfm1
    xmlAddAttribute "stopErp1" m.stopErp1
    xmlAddAttribute "stopCfm1" m.stopCfm1

    xmlAddAttribute "suspERP" m.suspERP
    xmlAddAttribute "suspCFM" m.suspCFM

    xmlElementEnd() -- Hinge2
)

-------------------------------------------------------------------------------
-- @name ExportJointSlider
--
-------------------------------------------------------------------------------
fn ExportJointSlider o m =
(
    xmlElementBegin "Slider"
    xmlAddAttribute "body1" o.name

    p = o.parent
    for m in p.modifiers do
    (
        if( m.name == "ODE Body (Mangalore)" ) then
        (
            xmlAddAttribute "body2" p.name
        )
    )

    --xmlAddAttribute "joint" ?
    xmlAddAttribute "anchor" o.pos
    
    xmlAddAttribute "axis" o.pos        -- default: axis(0.0, 1.0, 0.0)
    if( m.isLoStop0 ) then
    (
        xmlAddAttribute "lostop0" m.loStop0
    )
    if( m.isHiStop0 ) then
    (
        xmlAddAttribute "histop0" m.hiStop0
    )
    xmlAddAttribute "vel0" m.vel0
    xmlAddAttribute "fmax0" m.fMax0
    xmlAddAttribute "fudge0" m.fudge0
    xmlAddAttribute "bounce0" m.bounce0
    xmlAddAttribute "cfm0" m.cfm0
    xmlAddAttribute "stopErp0" m.stopErp0
    xmlAddAttribute "stopCfm0" m.stopCfm0

    xmlElementEnd() -- Hinge
)

-------------------------------------------------------------------------------
-- @name ExportJointUniversal
--
-------------------------------------------------------------------------------
fn ExportJointUniversal o m =
(
    xmlElementBegin "UniversalJoint"
    xmlAddAttribute "body1" o.name

    p = o.parent
    for m in p.modifiers do
    (
        if( m.name == "ODE Body (Mangalore)" ) then
        (
            xmlAddAttribute "body2" p.name
        )
    )

    --xmlAddAttribute "joint" ?
    xmlAddAttribute "anchor" o.pos

    -- axis #1
    xmlAddAttribute "axis1" o.pos        -- default: axis(0.0, 1.0, 0.0)
    if( m.isLoStop0 ) then
    (
        xmlAddAttribute "lostop0" m.loStop0
    )
    if( m.isHiStop0 ) then
    (
        xmlAddAttribute "histop0" m.hiStop0
    )
    xmlAddAttribute "vel0" m.vel0
    xmlAddAttribute "fmax0" m.fMax0
    xmlAddAttribute "fudge0" m.fudge0
    xmlAddAttribute "bounce0" m.bounce0
    xmlAddAttribute "cfm0" m.cfm0
    xmlAddAttribute "stopErp0" m.stopErp0
    xmlAddAttribute "stopCfm0" m.stopCfm0

    -- axis #2
    xmlAddAttribute "axis2" o.pos        -- default: axis(0.0, 1.0, 0.0)
    if( m.isLoStop1 ) then
    (
        xmlAddAttribute "lostop1" m.loStop1
    )
    if( m.isHiStop1 ) then
    (
        xmlAddAttribute "histop1" m.hiStop1
    )
    xmlAddAttribute "vel1" m.vel1
    xmlAddAttribute "fmax1" m.fMax1
    xmlAddAttribute "fudge1" m.fudge1
    xmlAddAttribute "bounce1" m.bounce1
    xmlAddAttribute "cfm1" m.cfm1
    xmlAddAttribute "stopErp1" m.stopErp1
    xmlAddAttribute "stopCfm1" m.stopCfm1

    xmlElementEnd() -- UniversalJoint
)

-------------------------------------------------------------------------------
-- @name ExportJointAMotor
--
-------------------------------------------------------------------------------
fn ExportJointAMotor o m =
(
    xmlElementBegin "AMotor"
    xmlAddAttribute "body1" o.name

    p = o.parent
    for m in p.modifiers do
    (
        if( m.name == "ODE Body (Mangalore)" ) then
        (
            xmlAddAttribute "body2" p.name
        )
    )

    --xmlAddAttribute "joint" ?
    xmlAddAttribute "anchor" o.pos

    -- compositeloader.cc �� ���� ������ 2���̴�.
    xmlAddAttribute "numAxis" 2

    -- axis #1
    xmlAddAttribute "axis1" o.pos        -- default: axis(0.0, 1.0, 0.0)
    if( m.isLoStop0 ) then
    (
        xmlAddAttribute "lostop0" m.loStop0
    )
    if( m.isHiStop0 ) then
    (
        xmlAddAttribute "histop0" m.hiStop0
    )
    xmlAddAttribute "vel0" m.vel0
    xmlAddAttribute "fmax0" m.fMax0
    xmlAddAttribute "fudge0" m.fudge0
    xmlAddAttribute "bounce0" m.bounce0
    xmlAddAttribute "cfm0" m.cfm0
    xmlAddAttribute "stopErp0" m.stopErp0
    xmlAddAttribute "stopCfm0" m.stopCfm0

    -- axis #2
    xmlAddAttribute "axis2" o.pos        -- default: axis(0.0, 1.0, 0.0)
    if( m.isLoStop1 ) then
    (
        xmlAddAttribute "lostop1" m.loStop1
    )
    if( m.isHiStop1 ) then
    (
        xmlAddAttribute "histop1" m.hiStop1
    )
    xmlAddAttribute "vel1" m.vel1
    xmlAddAttribute "fmax1" m.fMax1
    xmlAddAttribute "fudge1" m.fudge1
    xmlAddAttribute "bounce1" m.bounce1
    xmlAddAttribute "cfm1" m.cfm1
    xmlAddAttribute "stopErp1" m.stopErp1
    xmlAddAttribute "stopCfm1" m.stopCfm1

    xmlElementEnd() -- AMotor
)


-------------------------------------------------------------------------------
-- @name ExportODE
--
-- @todo
--
-------------------------------------------------------------------------------
fn ExportODE filename physicType =
(
    -- physics type
    isRagDoll = false;
    if( physicType == 2 )    then isRagDoll = true;

    -- create file
    outfile = createFile filename;
    if( outfile == undefined ) then
    (
        messageBox("Error: cannot create file \"" + filename + "\"");
        return false;
    )

    local odeBodyArray = #();
    local odeJointArray = #();

    -- collect ode body
    for o in objects do
    (
        if( o != undefined ) then
        (
            for m in o.modifiers do
            (
                if( m.name == "ODE Body (Mangalore)" ) then
                (
                    __odebody = ODEBody o m;
                    append odeBodyArray __odebody;
                )
            )
        )
    )

    -- collect ode joint
    for odeBody in odeBodyArray do
    (
        if( odeBody.m.hasJoint ) then
        (
            append odeJointArray odeBody
        )
    )

    -- XML Begin
    xmlBegin outfile;

    xmlElementBegin "Composite"
    if( isRagDoll ) then
    (
        xmlAddAttribute "type" "ragdoll"
    )
    else
    (
        xmlAddAttribute "type" "standard"
    )
    xmlAddAttribute "numBodies" odeBodyArray.count;
    xmlAddAttribute "numJoints" odeJointArray.count;
    xmlAddAttribute "numMeshes" 0;
    --xmlAddAttribute "numShapes" 0;
        
    -- todo : count shape
    -- todo : mesh ?

    -- Export Rigid Body
    for odeBody in odeBodyArray do
    (
        ExportODEBody odeBody.o odeBody.m isRagDoll
    )

        -- Export Joint
    for odeJoint in odeJointArray do
    (
        ExportODEJoint odeJoint.o odeJoint.m
    )

    xmlElementEnd() -- Composite

    xmlEnd() -- XML End
)

-------------------------------------------------------------------------------
-- @name ODEExporter
--
-- @todo
-- 1. viewer
-------------------------------------------------------------------------------
utility ODEExporter "Mangalore Physics Exporter" rolledup:true
(
    group "Physic Type"
    (
       radiobuttons physicType labels:#("Standard", "Rag Doll") default:1  align:#left
    )

    button btnExport "Export" --height:15 width:150

    on btnExport pressed do
    (
        filename = getSaveFileName caption:"Select File to Export" types:"XML (*.xml)|*.xml"

        if(filename == undefined) then return 0;

                -- export file
        ExportODE filename physicType.state;

        -- todo : launcher viewer
    )
)

max utility mode
addrollout ODEExporter
max create mode

-------------------------------------------------------------------------------
-- EOF
-------------------------------------------------------------------------------
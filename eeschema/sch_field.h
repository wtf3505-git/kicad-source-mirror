/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2015 Jean-Pierre Charras, jp.charras at wanadoo.fr
 * Copyright (C) 2004-2019 KiCad Developers, see change_log.txt for contributors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#ifndef CLASS_SCH_FIELD_H
#define CLASS_SCH_FIELD_H


#include <eda_text.h>
#include <sch_item.h>
#include <template_fieldnames.h>
#include <general.h>


class SCH_EDIT_FRAME;
class SCH_COMPONENT;
class LIB_FIELD;


/**
 * Class SCH_FIELD
 * instances are attached to a component and provide a place for the component's value,
 * reference designator, footprint, and user definable name-value pairs of arbitrary purpose.
 *
 * <ul> <li>Field 0 is reserved for the component reference.</li>
 * <li>Field 1 is reserved for the component value.</li>
 * <li>Field 2 is reserved for the component footprint.</li>
 * <li>Field 3 is reserved for the component data sheet file.</li>
 * <li>Field 4 and higher are user defineable.</li></ul>
 */

class SCH_FIELD : public SCH_ITEM, public EDA_TEXT
{
    int      m_id;         ///< Field index, @see enum NumFieldType

    wxString m_name;

public:
    SCH_FIELD( const wxPoint& aPos, int aFieldId, SCH_COMPONENT* aParent,
               const wxString& aName = wxEmptyString );

    // Do not create a copy constructor.  The one generated by the compiler is adequate.

    ~SCH_FIELD();

    wxString GetClass() const override
    {
        return wxT( "SCH_FIELD" );
    }

    bool IsType( const KICAD_T aScanTypes[] ) override
    {
        if( SCH_ITEM::IsType( aScanTypes ) )
            return true;

        for( const KICAD_T* p = aScanTypes; *p != EOT; ++p )
        {
            if( *p == SCH_FIELD_LOCATE_REFERENCE_T && m_id == REFERENCE )
                return true;
            else if ( *p == SCH_FIELD_LOCATE_VALUE_T && m_id == VALUE )
                return true;
            else if ( *p == SCH_FIELD_LOCATE_FOOTPRINT_T && m_id == FOOTPRINT )
                return true;
            else if ( *p == SCH_FIELD_LOCATE_DATASHEET_T && m_id == DATASHEET )
                return true;
        }

        return false;
    }

    /**
     * Function GetName
     * returns the field name.
     *
     * @param aUseDefaultName When true return the default field name if the field name is
     *                        empty.  Otherwise the default field name is returned.
     * @return A wxString object containing the name of the field.
     */
    wxString GetName( bool aUseDefaultName = true ) const;

    void SetName( const wxString& aName ) { m_name = aName; }

    int GetId() const { return m_id; }

    void SetId( int aId ) { m_id = aId; }

    /**
     * Function GetFullyQualifiedText
     * returns the fully qualified field text by allowing for the part suffix to be added
     * to the reference designator field if the component has multiple parts.  For all other
     * fields this is the equivalent of EDA_TEXT::GetText().
     *
     * @return a const wxString object containing the field's string.
     */
    const wxString GetFullyQualifiedText() const;

    void Place( SCH_EDIT_FRAME* frame, wxDC* DC );

    const EDA_RECT GetBoundingBox() const override;

    /**
     * Function IsHorizJustifyFlipped
     * Returns whether the field will be rendered with the horizontal justification
     * inverted due to rotation or mirroring of the parent.
     */
    bool IsHorizJustifyFlipped() const;

    /**
     * Function IsVoid
     * returns true if the field is either empty or holds "~".
     */
    bool IsVoid() const;

    void SwapData( SCH_ITEM* aItem ) override;

    /**
     * Function ImportValues
     * copy parameters from a LIB_FIELD source.
     * Pointers and specific values (position) are not copied
     * @param aSource = the LIB_FIELD to read
     */
    void ImportValues( const LIB_FIELD& aSource );

    int GetPenSize() const override;

    void Print( wxDC* aDC, const wxPoint& aOffset ) override;

    void Move( const wxPoint& aMoveVector ) override
    {
        Offset( aMoveVector );
    }

    void Rotate( wxPoint aPosition ) override;

    /**
     * @copydoc SCH_ITEM::MirrorX()
     *
     * This overload does nothing.  Fields are never mirrored alone.  They are moved
     * when the parent component is mirrored.  This function is only needed by the
     * pure function of the master class.
     */
    void MirrorX( int aXaxis_position ) override
    {
    }

    /**
     * @copydoc SCH_ITEM::MirrorY()
     *
     * This overload does nothing.  Fields are never mirrored alone.  They are moved
     * when the parent component is mirrored.  This function is only needed by the
     * pure function of the master class.
     */
    void MirrorY( int aYaxis_position ) override
    {
    }

    bool Matches( wxFindReplaceData& aSearchData, void* aAuxData ) override;

    bool Replace( wxFindReplaceData& aSearchData, void* aAuxData = NULL ) override;

    wxString GetSelectMenuText( EDA_UNITS_T aUnits ) const override;

    BITMAP_DEF GetMenuImage() const override;

    bool IsReplaceable() const override;

    wxPoint GetLibPosition() const { return EDA_TEXT::GetTextPos(); }

    wxPoint GetPosition() const override;
    void SetPosition( const wxPoint& aPosition ) override;

    bool HitTest( const wxPoint& aPosition, int aAccuracy = 0 ) const override;
    bool HitTest( const EDA_RECT& aRect, bool aContained, int aAccuracy = 0 ) const override;

    void Plot( PLOTTER* aPlotter ) override;

    EDA_ITEM* Clone() const override;

#if defined(DEBUG)
    void Show( int nestLevel, std::ostream& os ) const override { ShowDummy( os ); }
#endif
};


#endif /* CLASS_SCH_FIELD_H */
